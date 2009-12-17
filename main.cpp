#include <wx/wx.h>
#include <wx/file.h>
#include <wx/stdpaths.h>
#include <wx/timer.h>
#include <iostream>
#include <rice/detail/ruby.hpp>

#ifdef __WXMAC__
#include <ApplicationServices/ApplicationServices.h>
#endif

#include "wx_utils.cpp"
#include "ruby_utils.cpp"
#include "bowline/bowline.cpp"

extern "C" {
  void Init_prelude(void);
}

class App : public wxApp
{
public:
  bool OnInit();
  int OnExit();
  void InitRuby();
  void Tick(wxTimerEvent& evt);
  void Idle(wxIdleEvent& evt);
  wxString ResourcePath();
  BowlineMainWindow* window;
  wxTimer tickTimer;
};

IMPLEMENT_APP(App)

bool App::OnInit()
{
  window = new BowlineMainWindow();
  
  InitRuby();
  InitBowline();
  
  int error;
  rb_load_protect(rb_str_new2("script/init"), Qfalse, &error);
  if(error){
    RubyUtils::LogError();
    throw "Ruby Error";
  }
    
  // Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(App::Idle));
  tickTimer.Connect(tickTimer.GetId(), wxEVT_TIMER, wxTimerEventHandler(App::Tick));
  tickTimer.Start(50);

  window->SetupConfiguration();
  window->Show();
  
  #ifdef __WXMAC__
  // Enable interaction for unbundled apps
  ProcessSerialNumber PSN;
  GetCurrentProcess(&PSN);
  TransformProcessType(&PSN,kProcessTransformToForegroundApplication);
  SetFrontProcess(&PSN);
  #endif
  
  return true;
}

int App::OnExit()
{
  tickTimer.Stop();
  // TODO cleaning up Ruby causes a segfault, 
  // probably because the tickTimer is still executing
  // ruby_finalize();
  return 0;
}

void App::InitRuby(){
  RUBY_INIT_STACK;
  ruby_init();
  ruby_script("bowline");
  ruby_init_loadpath();

  // Since ruby_init_gems is not public
  rb_define_module("Gem");
  Init_prelude();
  
  wxString resource_path = this->ResourcePath();  
  ruby_incpush(resource_path.c_str());
}

void App::Tick(wxTimerEvent& WXUNUSED(evt)){
  rb_eval_string_protect("Bowline::Desktop.tick", NULL);
}

// void App::Idle(wxIdleEvent& WXUNUSED(evt)) {
//   This absolutely sucks. Needed to get threads running.
//   There is a Ruby API to do this properly (see RubyGVL),
//   but it keeps segfaulting on me
//   rb_eval_string_protect("sleep(0.05)", NULL);
//   rb_eval_string_protect("Bowline::Desktop.idle", NULL);
// }

wxString App::ResourcePath(){
  if(App::argc > 1){
    wxString argv1 = App::argv[1];
    if(argv1 == ".") return wxGetCwd();
    if(wxIsAbsolutePath(argv1)) return argv1;
    return wxGetCwd() + wxFILE_SEP_PATH + argv1;
  } else {
    wxString path = wxStandardPaths::Get().GetResourcesDir();
    return path;
  }
}
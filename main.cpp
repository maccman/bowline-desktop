#include <wx/wx.h>
#include <wx/file.h>
#include <wx/stdpaths.h>
#include <wx/timer.h>
#include <ruby.h>
#include <iostream>

#ifdef __WXMAC__
#include <ApplicationServices/ApplicationServices.h>
#endif

#include "wx_utils.cpp"
#include "ruby_utils.cpp"
#include "bowline_config.cpp"
#include "main_frame.cpp"
#include "bowline_control.cpp"

extern "C" {
  void Init_prelude(void);
  VALUE App_RunScript(VALUE self, VALUE arg);
  extern VALUE rb_mKernel;
}

class App : public wxApp
{
public:
  virtual bool OnInit();
  virtual int OnExit();
  virtual void InitRuby();
  static MainFrame* frame;
  static BowlineControl* bowline;
  void Tick(wxTimerEvent& evt);
  void Idle(wxIdleEvent& evt);
  wxString ResourcePath();
  wxTimer tickTimer;
};

MainFrame* App::frame;
BowlineControl* App::bowline;

IMPLEMENT_APP(App)

bool App::OnInit()
{  
  this->InitRuby();

  // TODO - move this to BowlineControl, and use Rice
  rb_define_module_function(rb_mKernel, "run_js_script", RUBY_METHOD_FUNC(App_RunScript), 1);
  
  int error;
  rb_load_protect(rb_str_new2("script/init"), Qfalse, &error);
  if(error){
    RubyUtils::LogError();
    Exit();
  }
  
  wxSize coords = wxSize(BowlineConfig::getInt(_("width")), BowlineConfig::getInt(_("height")));
  wxString appName = BowlineConfig::getString(_("name"));
  bool chrome = BowlineConfig::getBool(_("chrome"));
  App::frame = new MainFrame(appName, coords, chrome);
    
  App::bowline = new BowlineControl(App::frame);
  bowline->LoadURL("file://" + BowlineConfig::getString(_("index_path")));
  
  Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(App::Idle));
  tickTimer.Connect(tickTimer.GetId(), wxEVT_TIMER, wxTimerEventHandler(App::Tick));
  tickTimer.Start(50);

  App::frame->Show(true);
  SetTopWindow(App::frame);
  
  #ifdef __WXMAC__
  // Temporary measure to enable interaction
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
  ruby_finalize();
  return 0;
}

extern "C" VALUE App_RunScript(VALUE self, VALUE arg){
  if(!App::bowline) return Qnil;
  // std::cout << "run_script: " << StringValueCStr(arg) << std::endl;
  wxString result = App::bowline->RunScript(StringValueCStr(arg));
  return(rb_str_new2(result.c_str()));
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

void App::Idle(wxIdleEvent& WXUNUSED(evt)) {
  // This absolutely sucks. Needed to get threads running.
  // There is a Ruby API to do this properly (see RubyGVL),
  // but it keeps segfaulting on me
  // rb_eval_string_protect("sleep(0.05)", NULL);
  // rb_eval_string_protect("Bowline::Desktop.idle", NULL);
}

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
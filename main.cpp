#include <wx/wx.h>
#include <ruby.h>
#include <iostream>

#ifdef __WXMAC__
#include <ApplicationServices/ApplicationServices.h>
#endif

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
  void Idle(wxIdleEvent& evt);
  void Loaded(wxWebKitBeforeLoadEvent& evt);
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
  App::frame = new MainFrame(appName, coords);
  
  Connect(wxID_ANY, wxEVT_WEBKIT_BEFORE_LOAD, wxWebkitBeforeLoadEventHandler(App::Loaded));
  
  App::bowline = new BowlineControl(App::frame);
  bowline->LoadURL("file://" + BowlineConfig::getString(_("index_path")));
  
  Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(App::Idle));

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
  ruby_finalize();
  return 0;
}

extern "C" VALUE App_RunScript(VALUE self, VALUE arg){
  if(!App::bowline) return Qnil;
  // std::cout << "run_script: " << StringValueCStr(arg) << std::endl;
  return(
    rb_str_new2(
      App::bowline->RunScript(StringValueCStr(arg)).c_str()
    )
  );
}

void App::InitRuby(){
  RUBY_INIT_STACK;
  ruby_init();
  ruby_script("bowline");
  ruby_init_loadpath();

  // Since ruby_init_gems is not public
  rb_define_module("Gem");
  Init_prelude();

  ruby_incpush(wxGetCwd().c_str());
}

void App::Idle(wxIdleEvent& WXUNUSED(evt)) {
  rb_eval_string_protect("Bowline::Desktop.idle", NULL);
  // This absolutely sucks. Needed to get threads running.
  // There is a Ruby API to do this properly (see RubyGVL),
  // but it keeps segfaulting on me
  rb_eval_string_protect("sleep(0.05)", NULL);
}

void App::Loaded(wxWebKitBeforeLoadEvent& WXUNUSED(evt)){
  rb_eval_string_protect("Bowline::Desktop.loaded", NULL);
}
// Ruby & C++ - http://www.angelfire.com/electronic2/issac/rb_cpp_ext_tut.txt

#include <wx/wx.h>
#include <ruby.h>
#include <iostream>

#include "main_frame.cpp"
#include "bowline_control.cpp"

extern "C" {
  void Init_prelude(void);
  VALUE App_RunScript(VALUE self, VALUE arg);
}

class App : public wxApp
{
public:
  virtual bool OnInit();
  virtual int OnExit();
  virtual void InitRuby();
  static wxString App::RunScript(const wxString& js);
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
  // Show splash screen
  // http://docs.wxwidgets.org/stable/wx_wxsplashscreen.html#wxsplashscreen

  this->InitRuby();

  App::frame   = new MainFrame(wxPoint(50, 50), wxSize(450, 350));
  App::bowline = new BowlineControl(App::frame);

  VALUE rb_mBowline = rb_define_module("Bowline");
  rb_define_module_function(rb_mBowline, "run_script", RUBY_METHOD_FUNC(App_RunScript), 1);
  
  Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(App::Idle));
  bowline->Connect(wxID_ANY, wxEVT_WEBKIT_BEFORE_LOAD, wxWebkitBeforeLoadEventHandler(App::Loaded));

  // Rice::define_module("Bowline").define_module_function("execute", &App::RunScript);

  rb_require("init");
  
  bowline->LoadURL("file://" + wxGetCwd() + "/index.html");

  // Create event listener for ruby procs that need to be executed in the main thread

  App::frame->Show(true);
  SetTopWindow(App::frame);
  
  return true;
}

int App::OnExit()
{
  ruby_finalize();
  return 0;
}

extern "C" VALUE App_RunScript(VALUE self, VALUE arg){
  return(rb_str_new2(App::bowline->RunScript(StringValueCStr(arg)).c_str()));
}

wxString App::RunScript(const wxString& js){
  return(App::bowline->RunScript(js));
}

void App::InitRuby(){
  RUBY_INIT_STACK;
  ruby_init();
  ruby_script("Bowline");
  ruby_init_loadpath();

  // Since ruby_init_gems is not public
  rb_define_module("Gem");
  Init_prelude();

  ruby_incpush(wxGetCwd().c_str());
}

void App::Idle(wxIdleEvent& WXUNUSED(evt)) {
  rb_eval_string_protect("Bowline.idle", NULL);
  // This absolutely sucks. Needed to get threads running.
  // There is a Ruby API to do this properly (see RubyGVL),
  // but it keeps segfaulting on me
  rb_eval_string_protect("sleep(0.05)", NULL);
}

void App::Loaded(wxWebKitBeforeLoadEvent& WXUNUSED(evt)){
  rb_eval_string_protect("Bowline.loaded", NULL);
}
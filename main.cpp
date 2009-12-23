#include <wx/wx.h>
#include <wx/file.h>
#include <wx/stdpaths.h>
#include <wx/timer.h>
#include <iostream>
#include <rice/detail/ruby.hpp>

#ifdef __WXMAC__
#include <ApplicationServices/ApplicationServices.h>
#endif

#include "wx_pathname.cpp"
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
  void AddLoadPath(const wxPathname& path);
  void Tick(wxTimerEvent& evt);
  void Idle(wxIdleEvent& evt);
  wxPathname ResourcePath();
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
  // ruby_init_loadpath();
        
  wxPathname resource_path = this->ResourcePath();
  wxPathname rubylib_path  = wxPathname::Join(resource_path, "vendor", "rubylib");
  
  wxString version  = "1.9.1";
  // TODO - platform specific
  wxString platform = "i386-darwin9.8.0";
    
  // AddLoadPath(wxPathname::Join(rubylib_path, version));                          // RUBY_LIB
  // AddLoadPath(wxPathname::Join(rubylib_path, version, platform));                // RUBY_ARCHLIB
  // AddLoadPath(wxPathname::Join(rubylib_path, "site_path"));                      // RUBY_SITE_LIB
  // AddLoadPath(wxPathname::Join(rubylib_path, "site_path", version));             // RUBY_SITE_LIB2
  // AddLoadPath(wxPathname::Join(rubylib_path, "site_path", version, platform));   // RUBY_SITE_ARCHLIB
  // AddLoadPath(wxPathname::Join(rubylib_path, "vendor_ruby"));                    // RUBY_VENDOR_LIB
  // AddLoadPath(wxPathname::Join(rubylib_path, "vendor_ruby", version));           // RUBY_VENDOR_LIB2
  // AddLoadPath(wxPathname::Join(rubylib_path, "vendor_ruby", version, platform)); // RUBY_VENDOR_ARCHLIB
  // AddLoadPath(resource_path);
  
  ruby_incpush("/Users/Alex/bowline-desktop/vendor/rubylib/1.9.1");
  ruby_incpush("/Users/Alex/bowline-desktop/vendor/rubylib/1.9.1/i386-darwin9.8.0");
  ruby_incpush("/Users/Alex/bowline-desktop/vendor/rubylib/site_path");
  ruby_incpush("/Users/Alex/bowline-desktop/vendor/rubylib/site_path/1.9.1");
  ruby_incpush("/Users/Alex/bowline-desktop/vendor/rubylib/site_path/1.9.1/i386-darwin9.8.0");
  ruby_incpush("/Users/Alex/bowline-desktop/vendor/rubylib/vendor_ruby");
  ruby_incpush("/Users/Alex/bowline-desktop/vendor/rubylib/vendor_ruby/1.9.1");
  ruby_incpush("/Users/Alex/bowline-desktop/vendor/rubylib/vendor_ruby/1.9.1/i386-darwin9.8.0");
  ruby_incpush("/Users/Alex/bowline-desktop");
  
  Init_prelude();
}

void App::AddLoadPath(const wxPathname& pathname){
  wxString path = pathname.GetPath();
  char cpath[path.size() + 1];
  strcpy(cpath, path.c_str());
  ruby_incpush(cpath);
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

wxPathname App::ResourcePath(){
  if(App::argc > 1){
    wxString argv1 = App::argv[1];
    wxPathname cwd(wxGetCwd());
    if(argv1 == ".") return cwd;
    if(wxIsAbsolutePath(argv1)) return argv1;
    return wxPathname::Join(cwd, argv1);
  } else {
    wxString path = wxStandardPaths::Get().GetResourcesDir();
    return wxPathname(path);
  }
}
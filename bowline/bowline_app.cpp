#include "bowline.h"
#include <wx/utils.h>

using namespace Rice;

void bowline_app_exit(){
  wxWindow *window = wxTheApp->GetTopWindow();
  if (window) {
    window->Close(false);
  }
}

void bowline_app_busy(bool flag = true){
  if(flag){
    wxBeginBusyCursor();
  } else {
    wxEndBusyCursor();
  }
}

bool bowline_app_is_active(){
  return wxTheApp->IsActive();
}

void Init_Bowline_App(){
  Module rb_mBowline        = define_module("Bowline");
  Module rb_mBowlineDesktop = define_module_under(rb_mBowline, "Desktop");
  
  Module rb_mBowlineApp =
    define_module_under(rb_mBowlineDesktop, "App")
    .define_module_function("exit", &bowline_app_exit)
    .define_module_function("busy", &bowline_app_busy, Arg("flag") = true)
    .define_module_function("active?", &bowline_app_is_active);
}
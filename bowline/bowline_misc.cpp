#include "bowline.h"
#include <wx/utils.h>

using namespace Rice;

void bowline_misc_launch_browser(const wxString url){
  wxLaunchDefaultBrowser(url);
}

void bowline_misc_bell(){
  wxBell();
}

void Init_Bowline_Misc(){
  Module rb_mBowline        = define_module("Bowline");
  Module rb_mBowlineDesktop = define_module_under(rb_mBowline, "Desktop");
  
  Class rb_cBowlineMisc =
    define_class_under(rb_mBowlineDesktop, "Misc")
    .define_singleton_method("launch_browser", &bowline_misc_launch_browser)
    .define_singleton_method("bell", &bowline_misc_bell);
}
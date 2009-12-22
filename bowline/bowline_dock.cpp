#include "bowline.h"
#ifdef __WXMAC__
#include "badge_label.h"
#endif

using namespace Rice;

void bowline_dock_set_badge(wxString val){
  #ifdef __WXMAC__
  wxSetBadgeLabel(val);
  #endif
}

void bowline_dock_clear_badge(){
  #ifdef __WXMAC__
  wxRemoveBadgeLabel();
  #endif
}

void Init_Bowline_Dock(){
  Module rb_mBowline        = define_module("Bowline");
  Module rb_mBowlineDesktop = define_module_under(rb_mBowline, "Desktop");
  
  Module rb_mBowlineDock =
    define_class_under(rb_mBowlineDesktop, "Dock")
    .define_module_function("_badge=", &bowline_dock_set_badge)
    .define_module_function("clear_badge", &bowline_dock_clear_badge);
}
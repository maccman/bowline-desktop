// wxDialUpManager

#include "bowline.h"

using namespace Rice;

bool bowline_network_online(){
  // TODO
}

void Init_Bowline_Network(){
  Module rb_mBowline        = define_module("Bowline");
  Module rb_mBowlineDesktop = define_module_under(rb_mBowline, "Desktop");
  
  Class rb_cBowlineNetwork =
    define_class_under(rb_mBowlineDesktop, "Network")
    .define_singleton_method("online?", &bowline_network_online);
}
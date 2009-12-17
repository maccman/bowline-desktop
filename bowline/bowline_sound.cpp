#include "bowline.h"
#include <wx/sound.h>

using namespace Rice;

void bowline_sound_play(wxString path){
  wxSound(path).Play();
}

void Init_Bowline_Sound(){
  Module rb_mBowline        = define_module("Bowline");
  Module rb_mBowlineDesktop = define_module_under(rb_mBowline, "Desktop");
  
  Class rb_cBowlineSound =
    define_class_under(rb_mBowlineDesktop, "Sound")
    .define_singleton_method("play", &bowline_sound_play);
}
#include "bowline.h"
#include <wx/sound.h>

using namespace Rice;

void bowline_sound_play(wxString path){
  wxSound(path).Play();
}

void Init_Bowline_Sound(){
  Module rb_mBowline        = define_module("Bowline");
  Module rb_mBowlineDesktop = define_module_under(rb_mBowline, "Desktop");
  
  Module rb_mBowlineSound =
    define_module_under(rb_mBowlineDesktop, "Sound")
    .define_module_function("play", &bowline_sound_play);
}
#include "bowline.h"
#include "bowline_clipboard.cpp"
#include "bowline_host.cpp"
#include "bowline_window.cpp"
#include "bowline_app.cpp"
#include "bowline_misc.cpp"
#include "bowline_dialog.cpp"
#include "bowline_sound.cpp"
#include "bowline_dock.cpp"
#include "bowline_main_window.cpp"

void InitBowline(){
  Init_Bowline_Clipboard();
  Init_Bowline_Host();
  Init_Bowline_App();
  Init_Bowline_Misc();
  Init_Bowline_Dialog();
  Init_Bowline_Sound();
  Init_Bowline_Dock();
  Init_Bowline_Control();
  Init_Bowline_Window();
  Init_Bowline_Main_Window();
}
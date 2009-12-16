#include "bowline.h"
#include <wx/msgdlg.h>

using namespace Rice;

int bowline_dialog_message(
  const wxString message, 
  const wxString caption = "Message", 
  int style = wxOK
){
  return wxMessageBox(message, caption, style);
}

void Init_Bowline_Dialog(){
  Module rb_mBowline        = define_module("Bowline");
  Module rb_mBowlineDesktop = define_module_under(rb_mBowline, "Desktop");
  
  Class rb_cBowlineDialog =
    define_class_under(rb_mBowlineDesktop, "Dialog")
    .define_singleton_method("message", &bowline_dialog_message,
      (
        Arg("message"),
        Arg("caption") = (wxString)"Message",
        Arg("style") = wxOK
      )
    )
    .const_set("YES_NO",           wxYES_NO)
    .const_set("OK",               wxOK);
    
    // TODO 
    // weirdly if any of the following constants are defined we get a Ruby error:
    //  [BUG] object allocation during garbage collection phase
    // .const_set("CANCEL",           wxCANCEL)
    // .const_set("ICON_EXCLAMATION", wxICON_EXCLAMATION)
    // .const_set("ICON_HAND",        wxICON_HAND)
    // .const_set("ICON_ERROR",       wxICON_ERROR)
    // .const_set("QUESTION",         wxICON_QUESTION)
    // .const_set("INFORMATION",      wxICON_INFORMATION)
}
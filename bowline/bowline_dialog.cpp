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
  
  Module rb_mBowlineDialog =
    define_module_under(rb_mBowlineDesktop, "Dialog")
    .define_module_function("_message", &bowline_dialog_message,
      (
        Arg("message"),
        Arg("caption") = (wxString)"Message",
        Arg("style") = wxOK
      )
    )
    .const_set("YES_NO",           to_ruby(wxYES_NO))
    .const_set("OK",               to_ruby(wxOK))
    .const_set("CANCEL",           to_ruby(wxCANCEL))
    .const_set("ICON_EXCLAMATION", to_ruby(wxICON_EXCLAMATION))
    .const_set("ICON_HAND",        to_ruby(wxICON_HAND))
    .const_set("ICON_ERROR",       to_ruby(wxICON_ERROR))
    .const_set("QUESTION",         to_ruby(wxICON_QUESTION))
    .const_set("INFORMATION",      to_ruby(wxICON_INFORMATION));
}
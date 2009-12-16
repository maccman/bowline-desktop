#include "bowline.h"
#include <wx/clipbrd.h>

using namespace Rice;

bool bowline_clipboard_write(wxString data){
  if (wxTheClipboard->Open())
  {
    wxTheClipboard->SetData(new wxTextDataObject(data));
    wxTheClipboard->Close();
    return true;
  } else {
    return false;
  }
}

wxString bowline_clipboard_read(){
  wxTextDataObject data;
  if (wxTheClipboard->Open())
  {
    if (wxTheClipboard->IsSupported( wxDF_TEXT ))
    {
      wxTheClipboard->GetData( data );
    } else {
      return wxEmptyString;
    }
    wxTheClipboard->Close();
  } else {
    return wxEmptyString;
  }
  return data.GetText();
}

void Init_Bowline_Clipboard(){
  Module rb_mBowline        = define_module("Bowline");
  Module rb_mBowlineDesktop = define_module_under(rb_mBowline, "Desktop");

  Class rb_cBowlineClipboard =
    define_class_under(rb_mBowlineDesktop, "Clipboard")
    .define_singleton_method("write", &bowline_clipboard_write)
    .define_singleton_method("read", &bowline_clipboard_read);
}
#ifndef BOWLINE_WINDOW_CPP_PU66Y890
#define BOWLINE_WINDOW_CPP_PU66Y890

#include "bowline.h"
#include "bowline_control.cpp"
#include <wx/dirdlg.h>
#include <wx/filedlg.h>

using namespace Rice;

class BowlineWindow : public BowlineControl
{
public:
  BowlineWindow (
    const wxString path = wxEmptyString, 
    const wxString name = wxEmptyString,
    const bool chrome = true, 
    const wxSize size = wxDefaultSize
  ) : BowlineControl(wxTheApp->GetTopWindow(), path, name, chrome, size) {    
  }
};

// This is to stop Ruby GC the window, trying to delete
// it when it's already been deleted by C++
namespace Rice {
  template<>
  struct Default_Allocation_Strategy< BowlineWindow > {
    static void free(BowlineWindow * obj) { }
  };
}

void Init_Bowline_Window(){  
  Module rb_mBowline        = define_module("Bowline");
  Module rb_mBowlineDesktop = define_module_under(rb_mBowline, "Desktop");
  
  // TODO - default arguments for the constructor
  // (
  //   Arg("path") = wxEmptyString,
  //   Arg("name") = wxEmptyString,
  //   Arg("chrome") = true,
  //   Arg("size") = wxDefaultSize
  // )
  
  Class rb_cBowlineWindow = 
    define_class_under<BowlineWindow, BowlineControl>(rb_mBowlineDesktop, "Window")
    .define_constructor(Constructor<BowlineWindow, wxString, wxString, bool, wxSize>());
}

#endif /* end of include guard: BOWLINE_WINDOW_CPP_PU66Y890 */
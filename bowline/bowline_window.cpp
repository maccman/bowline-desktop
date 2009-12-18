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
  BowlineWindow() : BowlineControl(wxTheApp->GetTopWindow()){ 
  };  
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
  
  Class rb_cBowlineWindow = 
    define_class_under<BowlineWindow, BowlineControl>(rb_mBowlineDesktop, "Window")
    .define_constructor(Constructor<BowlineWindow>());
}

#endif /* end of include guard: BOWLINE_WINDOW_CPP_PU66Y890 */
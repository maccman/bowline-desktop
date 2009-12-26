#include "bowline.h"
#include "bowline_control.cpp"

using namespace Rice;

enum
{
  ID_Quit=1
};

class BowlineMainWindow : public BowlineControl
{
public:
  BowlineMainWindow(
    const wxString path = wxEmptyString, 
    const wxString name = wxEmptyString,
    const bool chrome = true, 
    const wxSize size = wxDefaultSize
  ) : BowlineControl(NULL, path, name, chrome, size) 
  {
    BowlineMainWindow::SetInstance(this);
    wxTheApp->SetTopWindow(frame);
  };
    
  static BowlineMainWindow *GetInstance(){ return mainWindowInstance; }
  static void SetInstance(BowlineMainWindow *win) { mainWindowInstance = win; }
  
protected:
  static BowlineMainWindow *mainWindowInstance;
  wxMenu *fileMenu;
};

BowlineMainWindow *BowlineMainWindow::mainWindowInstance = NULL;

void Init_Bowline_Main_Window(){
  Module rb_mBowline        = define_module("Bowline");
  Module rb_mBowlineDesktop = define_module_under(rb_mBowline, "Desktop");
  
  Class rb_cBowlineMainWindow =
    define_class_under<BowlineMainWindow, BowlineControl>(rb_mBowlineDesktop, "MainWindow")
    .define_singleton_method("get", &BowlineMainWindow::GetInstance);
}
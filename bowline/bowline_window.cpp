#include "bowline/bowline.h"
#include <wx/dirdlg.h>
#include <wx/filedlg.h>

using namespace Rice;

class BowlineWindow : public wxWindow
{
public:
  BowlineWindow (
    const wxString path, 
    const bool chrome = true, 
    const wxSize& size = wxDefaultSize,
    const wxWindow& parent = App::frame
  ) : wxWindow(parent, wxID_ANY, wxDefaultPosition, size) {
    
    bowline = new BowlineControl(this);
    bowline->LoadURL("file://" + path));
    
    // Add Webkit window
    Show();
  }
  
  wxString RunScript(wxString js){
    return bowline->RunScript(js);
  }
  
  wxString SelectDir(
    const wxString& message = "Choose a directory",
    const wxString& default_path = wxEmptyString
  ) {
    const wxString& path = wxDirSelector(
      message, 
      default_path, 
      wxDD_DEFAULT_STYLE, 
      wxDefaultPosition, 
      this
    );
    return path;
  }
  
  wxString SelectFile(
    const wxString& message = "Choose a file", 
    const wxString& default_path = wxEmptyString,
    const wxString& default_filename = wxEmptyString,
    const wxString& default_extension = wxEmptyString,
    const wxString& wildcard = "*.*",
    int flags = 0
  ){
    const wxString& path = wxFileSelector(
      message,
      default_path,
      default_filename,
      default_extension,
      wildcard,
      flags,
      this
    )
    return path;
  }
private:
  BowlineControl* bowline;
  /* data */
};

extern "C" 
void Init_Bowline_Window(){
  Data_Type<BowlineWindow> Class rb_cBowlineWindow =
    define_class<BowlineWindow>("Bowline::Window")
     .define_constructor(Constructor<BowlineWindow>())
     .define_method("center",      &BowlineWindow::Center)
     .define_method("close",       &BowlineWindow::Close)
     .define_method("disable",     &BowlineWindow::Disable)
     .define_method("enable",      &BowlineWindow::Enable)
     .define_method("id",          &BowlineWindow::GetId)
     .define_method("make_modal",  &BowlineWindow::MakeModal)
     .define_method("run_script",  &BowlineWindow::RunScript)
     .define_method("select_dir",  &BowlineWindow::SelectDir)
     .define_method("select_file", &BowlineWindow::SelectFile);
}
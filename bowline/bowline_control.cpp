#ifndef BOWLINE_CONTROL_CPP_F02I8V7L
#define BOWLINE_CONTROL_CPP_F02I8V7L

#include <wx/frame.h>
#include <wx/html/webkit.h>

// Not defined in webkit.h
#define wxWebkitBeforeLoadEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxWebKitBeforeLoadEventFunction, &func)

class BowlineControl : public wxFrame
{
public:
  BowlineControl (
    wxWindow* parent,
    const wxString path = wxEmptyString, 
    const wxString name = wxEmptyString,
    const bool chrome = true, 
    const wxSize size = wxDefaultSize
  ) : wxFrame(parent, wxID_ANY, name, wxDefaultPosition, size) {
    
    menuBar = new wxMenuBar;
    SetMenuBar(menuBar);
    
    webkit = new wxWebKitCtrl(this, wxID_ANY, wxEmptyString);
    LoadFile(path);
  }
  
  void LoadFile(wxString path){
    if(path != wxEmptyString)
      webkit->LoadURL("file://" + path);
  }
  
  void SetChrome(bool flag){
    // TODO
  }

  wxString RunScript(wxString js){
    return webkit->RunScript(js);
  }

  wxString SelectDir(
    const wxString message = "Choose a directory",
    const wxString default_path = wxEmptyString
  ) {
    wxString path = wxDirSelector(
      message, 
      default_path, 
      wxDD_DEFAULT_STYLE, 
      wxDefaultPosition, 
      this
    );    
    return path;
  }

  // TODO extend to return multiple files
  wxString SelectFile(
    const wxString message = "Choose a file", 
    const wxString default_path = wxEmptyString,
    const wxString default_filename = wxEmptyString,
    const wxString default_extension = wxEmptyString,
    const wxString wildcard = "*.*",
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
    );
    return path;
  }
  
  void SetHeight(int height){
    // TODO
  }
  
  void SetWidth(int width){
    // TODO
  }

protected:
  wxWebKitCtrl* webkit;
  wxMenuBar *menuBar;
};

void Init_Bowline_Control(){
  Class rb_cBowlineControl= 
    define_class<BowlineControl>("BowlineInternalControl")
     .define_method("center",      &BowlineControl::Center, Arg("direction") = wxBOTH)
     .define_method("close",       &BowlineControl::Close,  Arg("force") = false)
     .define_method("chrome=",     &BowlineControl::SetChrome)
     .define_method("disable",     &BowlineControl::Disable)
     .define_method("enable",      &BowlineControl::Enable, Arg("enable") = true)
     .define_method("file=",       &BowlineControl::LoadFile)
     .define_method("id",          &BowlineControl::GetId)
     .define_method("modal",       &BowlineControl::MakeModal, Arg("flag") = true)
     .define_method("name=",       &BowlineControl::SetName)
     .define_method("run_script",  &BowlineControl::RunScript)
     .define_method("raise",       &BowlineControl::Raise)
     .define_method("show",        &BowlineControl::Show,   Arg("show") = true)
     .define_method("height=",     &BowlineControl::SetHeight)
     .define_method("width=",      &BowlineControl::SetWidth)
     .define_method("select_dir",  &BowlineControl::SelectDir, 
        (
          Arg("message") = (wxString)"Choose a directory", 
          Arg("default_path") = (wxString)wxEmptyString
        )
      )
     .define_method("select_file", &BowlineControl::SelectFile,
        (
          Arg("message")            = (wxString)"Choose a file",
          Arg("default_path")       = (wxString)wxEmptyString,
          Arg("default_filename")   = (wxString)wxEmptyString,
          Arg("default_extension")  = (wxString)wxEmptyString,
          Arg("wildcard")           = (wxString)"*.*",
          Arg("flags")              = 0
        )
      )
      .const_set("FD_OPEN",             wxFD_OPEN)
      .const_set("FD_SAVE",             wxFD_SAVE)
      .const_set("FD_OVERWRITE_PROMPT", wxFD_OVERWRITE_PROMPT);
      
      // TODO 
      // weirdly wxFD_FILE_MUST_EXIST causes the following Ruby error:
      //  [BUG] object allocation during garbage collection phase
      // .const_set("FD_FILE_MUST_EXIST",  wxFD_FILE_MUST_EXIST)
}

#endif /* end of include guard: BOWLINE_CONTROL_CPP_F02I8V7L */

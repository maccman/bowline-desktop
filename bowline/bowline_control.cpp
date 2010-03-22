#ifndef BOWLINE_CONTROL_CPP_F02I8V7L
#define BOWLINE_CONTROL_CPP_F02I8V7L

#include "bowline_webkit.h"

#include <wx/frame.h>
#include <wx/weakref.h>
    
#define FREED_RETURN if(frame == NULL) return
#define FREED_RETURN_OBJ(obj) if(frame == NULL) return obj

enum {
  ID_RELOAD = wxID_HIGHEST + 1,
  ID_SHOW_INSPECTOR
};

using namespace Rice;

class BowlineControl : public wxEvtHandler
{
public:
  BowlineControl () {
    BowlineControl(NULL);
  }
  
  BowlineControl (
    wxWindow* parent,
    const wxString path = wxEmptyString, 
    const wxString name = wxEmptyString,
    const bool chrome = true, 
    const wxSize size = wxDefaultSize
  ) {
    frame = new wxFrame(
      parent, 
      wxID_ANY, 
      name, 
      wxDefaultPosition, 
      size
    );
        
    wxMenu *editMenu = new wxMenu;
    editMenu->Append(wxID_CUT, _T("Cut\tCTRL+X"));
    editMenu->Append(wxID_COPY, _T("Copy\tCTRL+C"));
    editMenu->Append(wxID_PASTE, _T("Paste\tCTRL+V"));
    
    menuBar = new wxMenuBar();
    menuBar->Append(editMenu, _T("&Edit"));
    frame->SetMenuBar(menuBar);
    
    webkit = new BowlineWebKit(frame, wxID_ANY);
    webkit->Connect(wxID_ANY, wxEVT_WEBKIT_SCRIPT, wxWebKitScriptEventHandler(BowlineControl::OnScript), NULL, this);

    LoadFile(path);
  }
  
  void OnExit(wxCommandEvent& WXUNUSED(e)){
    wxTheApp->GetTopWindow()->Close(false);
  }
  
  void OnCut(wxCommandEvent& WXUNUSED(e))
  {
    FREED_RETURN;
    webkit->Cut();
  }

  void OnCopy(wxCommandEvent& WXUNUSED(e))
  {
    FREED_RETURN;
    webkit->Copy();
  }

  void OnPaste(wxCommandEvent& WXUNUSED(e))
  {
    FREED_RETURN;
    webkit->Paste();
  }
  
  void Reload(){
    FREED_RETURN;
    webkit->Reload();
  }
  
  void OnReload(wxCommandEvent& WXUNUSED(e))
  {
    Reload();
  }
  
  void OnShowInspector(wxCommandEvent& WXUNUSED(e)){
    ShowInspector();
  }
  
  void OnScript(wxWebKitScriptEvent& evt){
    if(scriptCallback) {
      scriptCallback.call("call", evt.GetData());
    }
  }
  
  void SetScriptCallback(Object proc){
    scriptCallback = proc;
  }
  
  void EnableDeveloper(){
    FREED_RETURN;
    wxMenu *developerMenu = new wxMenu;
    developerMenu->Append(ID_RELOAD, _T("Reload\tCTRL+R"));
    developerMenu->Append(ID_SHOW_INSPECTOR, _T("Show Inspector\tCTRL+I"));
    menuBar->Append(developerMenu, _T("&Developer"));
    webkit->EnableDeveloper();
  }
  
  void LoadFile(wxString path){
    FREED_RETURN;
    if(path != wxEmptyString)
      webkit->LoadURL("file://" + path);
  }
  
  void LoadURL(wxString url){
    FREED_RETURN;
    if(url != wxEmptyString)
      webkit->LoadURL(url);    
  }
  
  // TODO - Only works when you set these style flags on creation
  void SetChrome(bool flag){
    // FREED_RETURN;
    // if( !flag ) {
    //   frame->SetWindowStyle(wxBORDER_NONE|wxFRAME_NO_TASKBAR);
    // }
  }
  
  void SetSource(wxString source){
    if(!webkit) return;
    webkit->SetPageSource(source);
  }
  
  wxString GetSource(){
    if(!webkit) return wxEmptyString;
    return webkit->GetPageSource();
  }

  wxString RunScript(wxString js){
    if(!webkit) return wxEmptyString;
    return webkit->RunScript(js);
  }

  wxString SelectDir(
    const wxString message = "Choose a directory",
    const wxString default_path = wxEmptyString
  ) {
    FREED_RETURN_OBJ(wxEmptyString);
    wxString path = wxDirSelector(
      message, 
      default_path, 
      wxDD_DEFAULT_STYLE, 
      wxDefaultPosition, 
      frame
    );    
    return path;
  }

  // TODO extend to return multiple files
  wxString SelectFile(
    const wxString message = "Choose a file", 
    const wxString default_path = wxEmptyString,
    const wxString default_filename = wxEmptyString,
    const wxString default_extension = wxEmptyString,
    const wxString wildcard = ".",
    int flags = 0
  ){
    FREED_RETURN_OBJ(wxEmptyString);
    const wxString& path = wxFileSelector(
      message,
      default_path,
      default_filename,
      default_extension,
      wildcard,
      flags
    );
    
    return path;
  }
  
  void Center(int direction = wxBOTH){
    FREED_RETURN;
    frame->Center(direction);
  }
  
  void Close(){
    FREED_RETURN;
    frame->Close(false);
  }
  
  bool IsShown(){
    FREED_RETURN_OBJ(false);
    frame->IsShown();
  }
  
  void Show(){
    FREED_RETURN;
    frame->Show(true);
  }
  
  void Hide(){
    FREED_RETURN;
    frame->Show(false);
  }
  
  void Enable(){
    FREED_RETURN;
    frame->Enable(true);
  }
  
  void Disable(){
    FREED_RETURN;
    frame->Enable(false);
  }
  
  int GetId() const{
    FREED_RETURN_OBJ(-1);
    return frame->GetId();
  }
  
  void MakeModal(bool flag){
    FREED_RETURN;
    frame->MakeModal(flag);
  }
  
  void Raise(){
    FREED_RETURN;
    frame->Raise();
  }
  
  void SetTitle(wxString title){
    FREED_RETURN;
    frame->SetTitle(title);
  }
  
  void SetMinSize(int x, int y){
    FREED_RETURN;
    frame->SetMinSize(wxSize(x, y));
  }
  
  void SetMaxSize(int x, int y){
    FREED_RETURN;
    frame->SetMaxSize(wxSize(x, y));
  }
  
  void SetSize(int height, int width){
    FREED_RETURN;
    frame->SetSize(-1, -1, height, width, wxSIZE_USE_EXISTING);
  }
  
  void SetPosition(int x, int y){
    FREED_RETURN;
    frame->Move(x, y);
  }
  
  void ShowInspector(bool console = false){
    FREED_RETURN;
    webkit->ShowInspector(console);
  }

protected:
  DECLARE_DYNAMIC_CLASS(BowlineControl)
  DECLARE_EVENT_TABLE()
  
  wxWeakRef<wxFrame> frame;
  wxWeakRef<BowlineWebKit> webkit;
  wxMenuBar *menuBar;
  Object scriptCallback;
};

IMPLEMENT_DYNAMIC_CLASS(BowlineControl, wxEvtHandler)

BEGIN_EVENT_TABLE(BowlineControl, wxEvtHandler)
	EVT_MENU(wxID_EXIT,  BowlineControl::OnExit)
	EVT_MENU(wxID_CUT,   BowlineControl::OnCut)
	EVT_MENU(wxID_COPY,  BowlineControl::OnCopy)
	EVT_MENU(wxID_PASTE, BowlineControl::OnPaste)
	EVT_MENU(ID_RELOAD,  BowlineControl::OnReload)
	EVT_MENU(ID_SHOW_INSPECTOR, BowlineControl::OnShowInspector)
END_EVENT_TABLE()

void Init_Bowline_Control(){
  Class rb_cBowlineControl= 
    define_class<BowlineControl>("BowlineInternalControl")
     .define_method("_center",          &BowlineControl::Center, Arg("direction") = (int)wxBOTH)
     .define_method("close",            &BowlineControl::Close)
     .define_method("chrome=",          &BowlineControl::SetChrome)
     .define_method("disable",          &BowlineControl::Disable)
     .define_method("enable",           &BowlineControl::Enable)
     .define_method("enable_developer", &BowlineControl::EnableDeveloper)
     .define_method("_file=",           &BowlineControl::LoadFile)
     .define_method("_url=",            &BowlineControl::LoadURL)
     .define_method("source",           &BowlineControl::GetSource)
     .define_method("source=",          &BowlineControl::SetSource)
     .define_method("id",               &BowlineControl::GetId)
     .define_method("modal",            &BowlineControl::MakeModal, Arg("flag") = true)
     .define_method("title=",           &BowlineControl::SetTitle)
     .define_method("run_script",       &BowlineControl::RunScript)
     .define_method("raise",            &BowlineControl::Raise)
     .define_method("reload",           &BowlineControl::Reload)
     .define_method("show",             &BowlineControl::Show)
     .define_method("hide",             &BowlineControl::Hide)
     .define_method("set_size",         &BowlineControl::SetSize)
     .define_method("set_min_size",     &BowlineControl::SetMinSize)
     .define_method("set_max_size",     &BowlineControl::SetMaxSize)
     .define_method("set_position",     &BowlineControl::SetPosition)
     .define_method("show_inspector",   &BowlineControl::ShowInspector, Arg("console") = false)
     .define_method("script_callback=", &BowlineControl::SetScriptCallback)
     .define_method("_select_dir",      &BowlineControl::SelectDir, 
        (
          Arg("message") = (wxString)"Choose a directory", 
          Arg("default_path") = (wxString)wxEmptyString
        )
      )
     .define_method("_select_file", &BowlineControl::SelectFile,
        (
          Arg("message")            = (wxString)"Choose a file",
          Arg("default_path")       = (wxString)wxEmptyString,
          Arg("default_filename")   = (wxString)wxEmptyString,
          Arg("default_extension")  = (wxString)wxEmptyString,
          Arg("wildcard")           = (wxString)".",
          Arg("flags")              = 0
        )
      )
      .define_method("shown?",    &BowlineControl::IsShown)
      .const_set("FD_OPEN",             to_ruby((int)wxFD_OPEN))
      .const_set("FD_SAVE",             to_ruby((int)wxFD_SAVE))
      .const_set("FD_OVERWRITE_PROMPT", to_ruby((int)wxFD_OVERWRITE_PROMPT))
      .const_set("FD_FILE_MUST_EXIST",  to_ruby((int)wxFD_FILE_MUST_EXIST))
      .const_set("HORIZONTAL",          to_ruby((int)wxHORIZONTAL))
      .const_set("VERTICAL",            to_ruby((int)wxVERTICAL))
      .const_set("BOTH",                to_ruby((int)wxBOTH))
      .const_set("CENTRE_ON_SCREEN",    to_ruby((int)wxCENTRE_ON_SCREEN));
}

#endif /* end of include guard: BOWLINE_CONTROL_CPP_F02I8V7L */

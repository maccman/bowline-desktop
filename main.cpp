#include <wx/wx.h>
#include <wx/html/webkit.h>
#include <ruby.h>
#include <iostream>

#include "ruby_gvl.cpp"

extern "C" {
  void Init_prelude(void);
}

class App : public wxApp
{
    virtual bool OnInit();
    void InitRuby();
};

IMPLEMENT_APP(App)

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};

enum
{
    ID_Quit=1,
    ID_About
};


bool App::OnInit()
{
    // Show splash screen
    // http://docs.wxwidgets.org/stable/wx_wxsplashscreen.html#wxsplashscreen
    MainFrame *frame = new MainFrame( _("Bowline"), wxPoint(50, 50),
                                  wxSize(450, 350));
    
    wxWebKitCtrl* html = new wxWebKitCtrl(frame, wxID_ANY, _("http://google.com"));
    App::InitRuby();
    // Load ruby
    // Expose some webkit methods to Ruby
    // Load init.rb
    // Unlock Ruby (so threads can run)

    frame->Show(true);
    SetTopWindow(frame);
    
    return true;
}

void App::InitRuby(){
    RUBY_INIT_STACK;
    ruby_init();
    ruby_script("Embedded Ruby");
    ruby_init_loadpath();
  
    // Since ruby_init_gems is not public
    rb_define_module("Gem");
    Init_prelude();

    ruby_incpush(wxGetCwd().c_str());
    
    rb_require("init");
    
    RubyGVL::Unlock();

    // Call this sometime:
    // ruby_finalize();
}

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame( NULL, -1, title, pos, size )
{
    wxMenuBar *menuBar = new wxMenuBar;

    wxMenu *menuFile = new wxMenu;

    menuFile->Append( ID_About, _("&About...") );
    menuFile->AppendSeparator();
    menuFile->Append( ID_Quit, _("E&xit") );

    menuBar->Append(menuFile, _("&File") );

    SetMenuBar(menuBar);
    
    this->Connect( ID_Quit, wxEVT_COMMAND_MENU_SELECTED,
                    (wxObjectEventFunction) &MainFrame::OnQuit );
    this->Connect( ID_About, wxEVT_COMMAND_MENU_SELECTED,
                    (wxObjectEventFunction) &MainFrame::OnAbout );
}

void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox( _("wxWidgets Hello World example."), 
                  _("About Hello World"),
                  wxOK|wxICON_INFORMATION, this );
}
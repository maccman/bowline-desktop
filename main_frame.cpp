#include "main_frame.h"

MainFrame::MainFrame(wxString &name, const wxSize& size, bool chrome = true)
    : wxFrame( NULL, -1, name, wxDefaultPosition, size, chrome ? wxDEFAULT_FRAME_STYLE : wxSIMPLE_BORDER)
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
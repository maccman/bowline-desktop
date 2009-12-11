class MainFrame : public wxFrame
{
public:
    MainFrame(wxString& name, const wxSize& size);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};

enum
{
    ID_Quit=1,
    ID_About
};

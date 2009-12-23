#include <wx/filename.h>

class wxPathname : public wxFileName
{
public:
  wxPathname(const wxString& fullpath) : wxFileName(fullpath, wxEmptyString, wxPATH_NATIVE) {};
  
  static wxPathname Join(const wxPathname& dir1, const wxString& dir2){
    wxPathname path(dir1);
    path.AppendDir(dir2);
    return path;
  }
  
  static wxPathname Join(const wxPathname& dir1, const wxString& dir2, const wxString& dir3){
    wxPathname path(dir1);
    path.AppendDir(dir2);
    path.AppendDir(dir3);
    return path;
  }
  
  static wxPathname Join(const wxPathname& dir1, const wxString& dir2, const wxString& dir3, const wxString& dir4){
    wxPathname path(dir1);
    path.AppendDir(dir2);
    path.AppendDir(dir3);
    path.AppendDir(dir4);
    return path;
  }
};
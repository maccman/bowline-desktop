#include <wx/filename.h>

class wxPathname
{
public:  
  static wxString Join(const wxString& dir1,const wxString& dir2){
    wxFileName path(dir1, wxEmptyString, wxPATH_NATIVE);
    path.AppendDir(dir2);
    return path.GetPath();
  }
  
  static wxString Join(const wxString& dir1, const wxString& dir2, const wxString& dir3){
    wxFileName path(dir1, wxEmptyString, wxPATH_NATIVE);
    path.AppendDir(dir2);
    path.AppendDir(dir3);
    return path.GetPath();
  }
  
  static wxString Join(const wxString& dir1, const wxString& dir2, const wxString& dir3, const wxString& dir4){
    wxFileName path(dir1, wxEmptyString, wxPATH_NATIVE);
    path.AppendDir(dir2);
    path.AppendDir(dir3);
    path.AppendDir(dir4);
    return path.GetPath();
  }
};
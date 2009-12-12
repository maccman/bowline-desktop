class wxUtils
{
public:
  // Find the absolute path where this application has been run from.
  // argv0 is wxTheApp->argv[0]
  // cwd is the current working directory (at startup)
  static wxString FindAppPath(const wxString& argv0, const wxString& cwd)
  {
    wxString str;

    if (wxIsAbsolutePath(argv0)){
      return wxPathOnly(argv0);
    }
    else
    {
      // Is it a relative path?
      wxString currentDir(cwd);
      if (currentDir.Last() != wxFILE_SEP_PATH)
          currentDir += wxFILE_SEP_PATH;

      str = currentDir;

      // If argv0 starts with ./
      if(argv0.at(0) == '.'){
        str += argv0.Mid(2);
      } else {
        str += argv0;
      }

      if (wxFileExists(str))
          return wxPathOnly(str);
    }

    // It's neither an absolute path nor a relative path, search PATH.
    
    wxPathList pathList;
    pathList.AddEnvList(wxT("PATH"));
    str = pathList.FindAbsoluteValidPath(argv0);
    if (!str.IsEmpty())
        return wxPathOnly(str);

    // Failed
    return wxEmptyString;
  }
};

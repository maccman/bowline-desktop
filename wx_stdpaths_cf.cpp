#ifndef WX_STDPATHS_CF_CPP_T8MSQKZI
#define WX_STDPATHS_CF_CPP_T8MSQKZI

class wxStandardPathsCFExt : public wxStandardPathsCF
{
public:
  // On Mac, GetExecutablePath uses CFBundleCopyBundleURL, not CFBundleCopyExecutableURL
  // which means the function returns the path to the bundle, not the actual exe.
  virtual wxString GetRawExecutablePath() const {
    return GetFromFunc(CFBundleCopyExecutableURL);
  }
};

#endif /* end of include guard: WX_STDPATHS_CF_CPP_T8MSQKZI */
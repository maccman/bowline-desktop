#ifndef WEBSTRING_H_BXPHA24Q
#define WEBSTRING_H_BXPHA24Q

#include "config.h"
#include "PlatformString.h"

#include <wx/defs.h>
#include <wx/string.h>

// Bridge between wxString and WebCore::String
class WebString
{
public:
  WebString (WebCore::String str);  
  WebString (const wxString& str){ string = str; }
  
  operator WebCore::String();
  operator wxString() { return string; }
private:
  wxString string;
};

#endif /* end of include guard: WEBSTRING_H_BXPHA24Q */
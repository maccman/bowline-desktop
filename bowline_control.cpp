#include <wx/html/webkit.h>

// Not defined in webkit.h
#define wxWebkitBeforeLoadEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxWebKitBeforeLoadEventFunction, &func)

class BowlineControl : public wxWebKitCtrl 
{
public:
  BowlineControl(wxWindow *parent) : wxWebKitCtrl(parent, wxID_ANY, wxEmptyString) {}
};
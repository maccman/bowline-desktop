#ifndef WEBVIEWEXTERNAL_H_AY1Y1Q0N
#define WEBVIEWEXTERNAL_H_AY1Y1Q0N

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

extern const wxChar* wxWebViewNameStr;

class wxWebView : public wxWindow
{
public:

    wxWebView(wxWindow* parent, int id = wxID_ANY,
              const wxPoint& point = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style = 0,
              const wxString& name = wxWebViewNameStr);

    wxWebView();
    
    bool Create(wxWindow* parent, int id = wxID_ANY,
                const wxPoint& point = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxWebViewNameStr);
    
    virtual ~wxWebView();
    
    void LoadURL(const wxString& url);
    void Reload();
    
    wxString RunScript(const wxString& javascript);
};

#endif /* end of include guard: WEBVIEWEXTERNAL_H_AY1Y1Q0N */
#ifndef WEBKIT_H_UHFR9WO4
#define WEBKIT_H_UHFR9WO4

#include <wx/window.h>

class BowlineWebKit : public wxControl
{
public:
  DECLARE_DYNAMIC_CLASS(BowlineWebKit)
  
  BowlineWebKit() {};
  BowlineWebKit (wxWindow *parent,
                 wxWindowID winID,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize, long style = 0,
                 const wxValidator& validator = wxDefaultValidator,
                 const wxString& name = wxT("webkitctrl")) {
#ifdef __WXOSX_COCOA__
    m_webView   = NULL;
    m_inspector = NULL;
#endif
    developer   = false;

    Create(parent, winID, pos, size, style, validator, name);
  }
  virtual ~BowlineWebKit();
  
  bool Create(wxWindow *parent,
              wxWindowID winID,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize, long style = 0,
              const wxValidator& validator = wxDefaultValidator,
              const wxString& name = wxT("webkitctrl"));

  void SetPageSource(const wxString& source, const wxString& baseUrl = wxEmptyString);
  wxString GetPageSource();
  
  void LoadURL(const wxString&);
  wxString RunScript(const wxString&);
  void ShowInspector(bool console = false);
  void EnableDeveloper() {
    developer = true;
  }
  bool EnabledDeveloper() {
    return developer;
  }
  
  void Cut();
  void Copy();
  void Paste();
  void Reload();
    
#ifdef __WXGTK__
  virtual wxSize DoGetBestSize() const;
  virtual void DoApplyWidgetStyle(GtkRcStyle *style);
  //virtual GdkWindow *GTKGetWindow(wxArrayGdkWindows& windows) const;
#endif

#ifdef __WXOSX_COCOA__
  virtual void OnMouseMove(wxMouseEvent &event);
#endif
  
  bool developer;
  
#ifdef __WXOSX_COCOA__
  struct objc_object *m_webView;
  struct objc_object *m_inspector;
#endif

#ifdef __WXWIN__
	IWebView* m_webView;
#endif
};

/* Events */

class wxWebKitScriptEvent : public wxCommandEvent
{
    DECLARE_DYNAMIC_CLASS( wxWebKitScriptEvent )

public:
    wxString GetData() { return m_data; }
    void SetData(const wxString& data) { m_data = data; }

    wxWebKitScriptEvent( wxWindow* win = NULL );
    wxEvent *Clone(void) const { return new wxWebKitScriptEvent(*this); }

protected:
    wxString m_data;
};

typedef void (wxEvtHandler::*wxWebKitScriptEventFunction)(wxWebKitScriptEvent&);

#define wxWebKitScriptEventHandler( func ) \
    wxEVENT_HANDLER_CAST( wxWebKitScriptEventFunction, func )
    
wxDECLARE_EXPORTED_EVENT( WXDLLIMPEXP_CORE, wxEVT_WEBKIT_SCRIPT, wxWebKitScriptEvent );
      
#define EVT_WEBKIT_SCRIPT(func)                              \
            DECLARE_EVENT_TABLE_ENTRY( wxEVT_WEBKIT_SCRIPT, \
                            wxID_ANY, \
                            wxID_ANY, \
                            wxWebKitScriptEventFunction( func ), \
                            NULL ),

#endif /* end of include guard: WEBKIT_H_UHFR9WO4 */

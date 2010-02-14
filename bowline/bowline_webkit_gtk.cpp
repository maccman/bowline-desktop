#include "bowline_webkit.cpp"

bool BowlineWebKit::Create(wxWindow *parent,
                           wxWindowID winID,
                           const wxPoint& pos,
                           const wxSize& size, long style,
                           const wxValidator& validator,
                           const wxString& name){
                             
  if (!PreCreation( parent, pos, size ) ||
    !CreateBase( parent, winID, pos, size, style, validator, name ))
  {
    wxFAIL_MSG( wxT("BowlineWebKit creation failed") );
    return false;
  }
  
  m_webView = webkit_web_view_new();
  g_object_ref(m_webView);
  
  //   WebKitWebSettings* settings = webkit_web_settings_new();
  // g_object_set(G_OBJECT(settings), 
  //  "enable-developer-extras", TRUE,
  //  "enable-universal-access-from-file-uris", TRUE,
  //  "javascript-can-open-windows-automatically", FALSE,
  //  NULL);
  // webkit_web_view_set_settings(WEBKIT_WEB_VIEW(m_webView), settings);
  // 
  // const char* cUserAgent = 0;
  // g_object_get(G_OBJECT(settings), "user-agent", &cUserAgent, NULL);
  // userAgent.append(cUserAgent);
  // userAgent.append(" Bowline");
  // g_object_set(G_OBJECT(settings), "user-agent", userAgent.c_str(), NULL);
  
  
  m_parent->DoAddChild( this );

  PostCreation(size);
  
  // gtk_widget_show(m_webView);
  // gtk_container_add (GTK_CONTAINER (scrolled_window), web_view);
}

BowlineWebKit::~BowlineWebKit() { 
}

void BowlineWebKit::SetPageSource(const wxString& source){
}

wxString BowlineWebKit::GetPageSource(){
  return wxEmptyString;
}

void BowlineWebKit::LoadURL(const wxString& url){
  webkit_web_view_load_uri(WEBKIT_WEB_VIEW(m_webView), url);
}

wxString BowlineWebKit::RunScript(const wxString& javascript){
  return wxEmptyString;
}

void wxWebKitCtrl::ShowInspector(bool console){
}
#include "wx/wxprec.h"
#include "wx/splitter.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "bowline_webkit.cpp"
#include "webkit/webkitwebview.h"

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
  
  m_webView = WEBKIT_WEB_VIEW(webkit_web_view_new());
  g_object_ref(m_webView);
  
  // g_object_connect(G_OBJECT(m_webView),
  //    "signal::window-object-cleared",
  //    G_CALLBACK(WindowObjectClearedCallback), this,
  //    "signal::new-window-policy-decision-requested",
  //    G_CALLBACK(NewWindowPolicyDecisionCallback), this,
  //    "signal::populate-popup",
  //    G_CALLBACK(PopulatePopupCallback), this,
  //    "signal::create-web-view",
  //    G_CALLBACK(CreateWebViewCallback), this,
  //    "signal::script-alert",
  //    G_CALLBACK(ScriptAlertCallback), this->gtkWindow,
  //    "signal::script-confirm",
  //    G_CALLBACK(ScriptConfirmCallback), this->gtkWindow,
  //    "signal::script-prompt",
  //    G_CALLBACK(ScriptPromptCallback), this->gtkWindow,
  //    NULL);
	

  //   WebKitWebSettings* settings = webkit_web_settings_new();
  // g_object_set(G_OBJECT(settings), 
  //  "enable-developer-extras", TRUE,
  //  "enable-universal-access-from-file-uris", TRUE,
  //  "javascript-can-open-windows-automatically", FALSE,
  //  NULL);
  // webkit_web_view_set_settings(m_webView, settings);
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

void BowlineWebKit::SetPageSource(const wxString& source, const wxString& baseUrl){
}

wxString BowlineWebKit::GetPageSource(){
  return wxEmptyString;
}

void BowlineWebKit::LoadURL(const wxString& url){
  webkit_web_view_load_uri(m_webView, url);
}

wxString BowlineWebKit::RunScript(const wxString& javascript){
  return wxEmptyString;
}

void BowlineWebKit::ShowInspector(bool console){
}

void BowlineWebKit::Cut(){
}

void BowlineWebKit::Copy(){
}

void BowlineWebKit::Paste(){
}

void BowlineWebKit::Reload(){
}

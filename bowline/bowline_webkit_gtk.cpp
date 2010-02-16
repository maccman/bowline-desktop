#include <wx/wxprec.h>
#include <wx/splitter.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <webkit/webkitwebview.h>
#include <JavaScriptCore/JavaScript.h>
#include <webkit/webkitwebpolicydecision.h>
#include <webkit/webkitwebnavigationaction.h>
#include <webkit/webkitwebframe.h>
#include "bowline_webkit.cpp"

extern "C" {
  WEBKIT_API gchar* webkit_web_frame_dump_render_tree (WebKitWebFrame* frame);
}

char* UTF8CStringFromJSValueRef(JSContextRef context, JSValueRef stringRef)
{
    JSStringRef string = JSValueToStringCopy(context, stringRef, NULL);
    size_t sizeUTF8 = JSStringGetMaximumUTF8CStringSize(string);
    char* stringUTF8 = new char[sizeUTF8];
    JSStringGetUTF8CString(string, stringUTF8, sizeUTF8);
    JSStringRelease(string);
    return stringUTF8;
}

wxString wxStringFromJSValueRef(JSContextRef context, JSValueRef stringRef)
{
  return UTF8CStringFromJSValueRef(context, stringRef);
}

JSValueRef scriptDelegate(JSContextRef ctx, JSObjectRef func, JSObjectRef self, size_t argc, const JSValueRef argv[], JSValueRef* exception)
{
  if (argc < 1)
  {
      *exception = JSValueMakeNumber(ctx, 1);
      return NULL;
  }
  
  wxString data = wxStringFromJSValueRef(ctx ,argv[0]);
  
  BowlineWebKit* bowlineWebKit = (BowlineWebKit*) JSObjectGetPrivate(self);
  
  wxWebKitScriptEvent scriptEvent(bowlineWebKit);
  scriptEvent.SetData( data );
  if (bowlineWebKit->GetEventHandler())
      bowlineWebKit->GetEventHandler()->ProcessEvent( scriptEvent );
  
  JSValueRef jsUndefined = JSValueMakeUndefined(ctx);
  return jsUndefined;
}

static void WindowObjectClearedCallback(
	WebKitWebView* webView,
	WebKitWebFrame* frame,
	JSGlobalContextRef context,
	JSObjectRef window,
	gpointer data)
{
  JSStringRef objName = JSStringCreateWithUTF8CString("_app");
  JSObjectRef obj     = JSObjectMake(context, NULL, NULL);
  JSObjectSetProperty(context, window, objName, obj, kJSPropertyAttributeNone, NULL);
  JSStringRelease(objName);
  
  JSObjectSetPrivate(obj, data);
  
  JSStringRef funcName  = JSStringCreateWithUTF8CString("call");
  JSObjectRef func = JSObjectMakeFunctionWithCallback(context, funcName, scriptDelegate);
  JSObjectSetProperty(context, obj, funcName, func, kJSPropertyAttributeNone, NULL);
  JSStringRelease(funcName);
}

static gboolean NewWindowPolicyDecisionCallback(WebKitWebView* webView,
	WebKitWebFrame* frame, WebKitNetworkRequest* request,
	WebKitWebNavigationAction* navigationAction,
	WebKitWebPolicyDecision *policyDecision)
{
	webkit_web_policy_decision_ignore(policyDecision);
  return TRUE;
}

static gboolean ScriptAlertCallback(WebKitWebView* webView,
	WebKitWebFrame *frame, gchar* message, gpointer data)
{
  wxMessageBox(message, wxT("JavaScript Alert"), wxOK);
  return TRUE;
}

static gboolean ScriptConfirmCallback(WebKitWebView* webView,
	WebKitWebFrame* frame, gchar* message, gboolean* confirmed,
	gpointer data)
{
  wxMessageDialog dialog(NULL, message, wxT("JavaScript Confirm"), wxYES_NO);
  dialog.Centre();
  return(dialog.ShowModal() == wxID_YES);
}

static gboolean ScriptPromptCallback(WebKitWebView* webView,
	WebKitWebFrame* frame, gchar* message, gchar* defaultPromptValue,
	gchar** value, gpointer data)
{
  wxTextEntryDialog dialog(NULL, message, wxT("JavaScript Prompt"), wxEmptyString, wxOK | wxCANCEL);
  dialog.Centre();
  if (dialog.ShowModal() == wxID_OK) {
    wxString result(dialog.GetValue());
    // TODO
    // *value = result;
  } else {
    *value = NULL;
  }
  return TRUE;
}

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
  
  m_widget = webkit_web_view_new();
  g_object_ref(m_widget);
  
  g_object_connect(G_OBJECT(m_widget),
     "signal::window-object-cleared",
     G_CALLBACK(WindowObjectClearedCallback), this,
     "signal::new-window-policy-decision-requested",
     G_CALLBACK(NewWindowPolicyDecisionCallback), this,
     "signal::script-alert",
     G_CALLBACK(ScriptAlertCallback), this,
     "signal::script-confirm",
     G_CALLBACK(ScriptConfirmCallback), this,
     "signal::script-prompt",
     G_CALLBACK(ScriptPromptCallback), this,
     NULL);

  WebKitWebSettings* settings = webkit_web_settings_new();
  g_object_set(G_OBJECT(settings), 
   "enable-developer-extras", FALSE, // We don't have the inspector on GTK yet
   "enable-universal-access-from-file-uris", TRUE,
   "javascript-can-open-windows-automatically", FALSE,
   NULL);
  webkit_web_view_set_settings(WEBKIT_WEB_VIEW(m_widget), settings);

  std::string userAgent;
  const char* cUserAgent = 0;
  g_object_get(G_OBJECT(settings), "user-agent", &cUserAgent, NULL);
  userAgent.append(cUserAgent);
  userAgent.append(" Bowline");
  g_object_set(G_OBJECT(settings), "user-agent", userAgent.c_str(), NULL);
  
  m_parent->DoAddChild(this);
  PostCreation(size);
  
  return true;
}

BowlineWebKit::~BowlineWebKit() { 
}

void BowlineWebKit::SetPageSource(const wxString& source, const wxString& baseURL){
  webkit_web_view_load_html_string(WEBKIT_WEB_VIEW(m_widget), source, baseURL);
}

wxString BowlineWebKit::GetPageSource(){
  WebKitWebFrame* webFrame = webkit_web_view_get_main_frame(WEBKIT_WEB_VIEW(m_widget));
  return webkit_web_frame_dump_render_tree(webFrame);
}

void BowlineWebKit::LoadURL(const wxString& url){
  webkit_web_view_load_uri(WEBKIT_WEB_VIEW(m_widget), url);
}

wxString BowlineWebKit::RunScript(const wxString& scriptUTF8){
  WebKitWebFrame* webFrame = webkit_web_view_get_main_frame(WEBKIT_WEB_VIEW(m_widget));
  JSGlobalContextRef context = webkit_web_frame_get_global_context(webFrame);
  
  JSStringRef script = JSStringCreateWithUTF8CString(scriptUTF8);
  JSValueRef  result = JSEvaluateScript(context, script, NULL, NULL, 1, NULL);
  JSStringRelease(script);

  return wxStringFromJSValueRef(context, result);
}

void BowlineWebKit::ShowInspector(bool console){
  // TODO - inspector on GTK
}

void BowlineWebKit::Cut(){
  webkit_web_view_cut_clipboard(WEBKIT_WEB_VIEW(m_widget));
}

void BowlineWebKit::Copy(){
  webkit_web_view_copy_clipboard(WEBKIT_WEB_VIEW(m_widget));
}

void BowlineWebKit::Paste(){
  webkit_web_view_paste_clipboard(WEBKIT_WEB_VIEW(m_widget));
}

void BowlineWebKit::Reload(){
  webkit_web_view_reload(WEBKIT_WEB_VIEW(m_widget));
}

void BowlineWebKit::DoApplyWidgetStyle(GtkRcStyle *style)
{
  gtk_widget_modify_style(m_widget, style);
}

wxSize BowlineWebKit::DoGetBestSize() const
{
  return wxControl::DoGetBestSize();
}
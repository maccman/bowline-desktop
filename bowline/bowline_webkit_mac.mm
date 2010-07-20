#include "bowline_webkit.cpp"

#include <wx/osx/private.h>
#include <WebKit/WebKit.h>
#include <WebKit/WebUIDelegatePrivate.h>

@interface WebInspector : NSObject
{
    WebView *_webView;
}
- (id)initWithWebView:(WebView *)webView;
- (void)detach:(id)sender;
- (void)show:(id)sender;
- (void)showConsole:(id)sender;
@end

inline wxString wxStringWithNSString(NSString *nsstring)
{
#if wxUSE_UNICODE
    return wxString([nsstring UTF8String], wxConvUTF8);
#else
    return wxString([nsstring lossyCString]);
#endif // wxUSE_UNICODE
}

inline NSString* wxNSStringWithWxString(const wxString &wxstring)
{
#if wxUSE_UNICODE
    return [NSString stringWithUTF8String: wxstring.mb_str(wxConvUTF8)];
#else
    return [NSString stringWithCString: wxstring.c_str() length:wxstring.Len()];
#endif // wxUSE_UNICODE
}


@interface BowlineFrameLoadMonitor : NSObject
{
    BowlineWebKit* webKitWindow;
}

- initWithWxWindow: (BowlineWebKit*)inWindow;

@end

@interface BowlinePolicyDelegate : NSObject
{
    BowlineWebKit* webKitWindow;
}

- initWithWxWindow: (BowlineWebKit*)inWindow;

@end

@interface BowlineScriptDelegate : NSObject
{
    BowlineWebKit* webKitWindow;
    WebFrame *frame;
}

- initWithWxWindow: (BowlineWebKit*)inWindow forFrame: (WebFrame *)frame;

@end

@interface BowlineUIDelegate : NSObject
{
    BowlineWebKit* webKitWindow;
}

- initWithWxWindow: (BowlineWebKit*)inWindow;

@end


bool BowlineWebKit::Create(wxWindow *parent,
                           wxWindowID winID,
                           const wxPoint& pos,
                           const wxSize& size, long style,
                           const wxValidator& validator,
                           const wxString& name){
  
  m_macIsUserPane = false;                           
  wxControl::Create(parent, winID, pos, size, style, validator, name);
  
  SetBackgroundStyle(wxBG_STYLE_CUSTOM);
  
  NSRect rect = wxOSXGetFrameForControl(this, pos , size);
  m_webView = [[WebView alloc] initWithFrame:rect frameName:@"webkitFrame" groupName:@"webkitGroup"];

  m_peer = new wxWidgetCocoaImpl(this, m_webView);

  MacPostControlCreate(pos, size);
  
  [m_webView setHidden:false];
    
  WebPreferences *m_webPrefs = [[WebPreferences alloc] initWithIdentifier:@"bowline"];
  
  // Reduce memory footprint
  [m_webPrefs setCacheModel:WebCacheModelDocumentBrowser];
  
  [m_webPrefs setDeveloperExtrasEnabled:true];
	[m_webPrefs setPlugInsEnabled:YES]; 
	[m_webPrefs setDOMPasteAllowed:YES];
	[m_webPrefs setUserStyleSheetEnabled:NO];

	if ([m_webPrefs respondsToSelector:@selector(setDatabasesEnabled:)])
	{
		[m_webPrefs setDatabasesEnabled:YES];
	}
	if ([m_webPrefs respondsToSelector:@selector(setLocalStorageEnabled:)])
	{
		[m_webPrefs setLocalStorageEnabled:YES];
	}

	[m_webView setPreferences:m_webPrefs];
	[m_webPrefs release];
  
  // Register event listener interfaces
  BowlineFrameLoadMonitor* myFrameLoadMonitor = [[BowlineFrameLoadMonitor alloc] initWithWxWindow: this];
  [m_webView setFrameLoadDelegate:myFrameLoadMonitor];

  // This is used to veto page loads, etc.
  BowlinePolicyDelegate* myPolicyDelegate = [[BowlinePolicyDelegate alloc] initWithWxWindow: this];
  [m_webView setPolicyDelegate:myPolicyDelegate];
  
  // This is used for alerts, context menus etc
  BowlineUIDelegate* myUIDelegate = [[BowlineUIDelegate alloc] initWithWxWindow: this];
  [m_webView setUIDelegate:myUIDelegate];
  
  [m_webView setApplicationNameForUserAgent:@"Bowline"];
  // [m_webView registerURLSchemeAsLocal:@"app"];
  // [m_webView setBackgroundColor:[NSColor clearColor]];
  
  // Fix mouse move events
  wxTheApp->Connect(wxID_ANY, wxEVT_MOTION, wxMouseEventHandler(BowlineWebKit::OnMouseMove), NULL, this);
  
  return true;
}

BowlineWebKit::~BowlineWebKit() { 
  BowlineFrameLoadMonitor* myFrameLoadMonitor = [m_webView frameLoadDelegate];
  BowlinePolicyDelegate* myPolicyDelegate = [m_webView policyDelegate];
  [m_webView setFrameLoadDelegate: nil];
  [m_webView setPolicyDelegate: nil];
  
  if (myFrameLoadMonitor)
    [myFrameLoadMonitor release];
      
  if (myPolicyDelegate)
    [myPolicyDelegate release];

  if (m_inspector)
    [m_inspector release];
}

void BowlineWebKit::OnMouseMove(wxMouseEvent& WXUNUSED(evt)){
  // Apple's WebKit don't listen to mouseMoved events in the normal
  // fashion, but rather passes them through NSNotificationCenter.
  // However, this isn't working for some reason, I presume
  // 'WKSetNSWindowShouldPostEventNotifications' is meant to automatically
  // send NSMouseMovedNotification notifications on mouseMoved. 
  // However, since I can't see the source, we will have to generate the event.
  [[NSNotificationCenter defaultCenter]
      postNotificationName:@"NSMouseMovedNotification" object:nil
      userInfo:[NSDictionary dictionaryWithObject:[NSApp currentEvent] forKey:@"NSEvent"]];
}

void BowlineWebKit::SetPageSource(const wxString& source, const wxString& baseUrl){
  if( !m_webView )
      return;
      
  [[m_webView mainFrame] loadHTMLString:(NSString*)wxNSStringWithWxString( source ) baseURL:[NSURL URLWithString:wxNSStringWithWxString( baseUrl )]];
}

wxString BowlineWebKit::GetPageSource(){
  if( !m_webView )
      return wxEmptyString;

  WebDataSource* dataSource = [[m_webView mainFrame] dataSource];
  if ([[dataSource representation] canProvideDocumentSource]){
    return wxStringWithNSString( [[dataSource representation] documentSource] );
  }
  return wxEmptyString;
}

void BowlineWebKit::LoadURL(const wxString& url){
  if( !m_webView )
      return;

  [[m_webView mainFrame] loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:wxNSStringWithWxString(url)]]];
}

wxString BowlineWebKit::RunScript(const wxString& javascript){
  if ( !m_webView )
      return wxEmptyString;

  id result = [[m_webView windowScriptObject] evaluateWebScript:(NSString*)wxNSStringWithWxString( javascript )];

  NSString* resultAsString;
  wxString resultAsWxString = wxEmptyString;
  NSString* className = NSStringFromClass([result class]);
  if ([className isEqualToString:@"NSCFNumber"])
      resultAsString = [NSString stringWithFormat:@"%@", result];
  else if ([className isEqualToString:@"NSCFString"])
      resultAsString = result;
  else if ([className isEqualToString:@"NSCFBoolean"]){
      if ([result boolValue])
          resultAsString = @"true";
      else
          resultAsString = @"false";
  }
  else if ([className isEqualToString:@"WebScriptObject"])
      resultAsString = [result stringRepresentation];
  else if ([className isEqualToString:@"WebUndefined"])
      resultAsString = @"";
  else
      fprintf(stderr, "BowlineWebKit::RunScript - Unexpected return type: %s!\n", [className UTF8String]);
      resultAsString = @"";

  resultAsWxString = wxStringWithNSString( resultAsString );
  return resultAsWxString;
}

void BowlineWebKit::ShowInspector(bool console){
  if ( !m_webView )
      return;
  
  if( !m_inspector ){
    m_inspector = [[WebInspector alloc] initWithWebView:m_webView];
    [m_inspector detach:m_webView];
  }
  
  if(console){
    [m_inspector showConsole:m_webView];
  }
  else {
    [m_inspector show:m_webView];
  }
}

void BowlineWebKit::Cut(){
  [m_webView cut:m_webView];
}

void BowlineWebKit::Copy(){
  [m_webView copy:m_webView];
}

void BowlineWebKit::Paste(){
  [m_webView paste:m_webView];
}

void BowlineWebKit::Reload(){
  [m_webView reload:m_webView];
}

// Simple window for poups

class SimpleWindow : public wxFrame
{
public:
  SimpleWindow() : wxFrame(
    wxTheApp->GetTopWindow(), 
    wxID_ANY,
    wxEmptyString,
    wxDefaultPosition,
    wxDefaultSize
  ) {
    webkit = new BowlineWebKit(this, wxID_ANY);
  };
  virtual ~SimpleWindow() {};
  
  void LoadURL(wxString url){
    if (webkit == NULL) return;
    if(url != wxEmptyString)
      webkit->LoadURL(url);    
  }
  
  wxWeakRef<BowlineWebKit> webkit;
};

@implementation BowlineFrameLoadMonitor

- initWithWxWindow: (BowlineWebKit*)inWindow
{
  [super init];
  webKitWindow = inWindow;
  return self;
}

- (void)webView:(WebView *)webView didClearWindowObject:(WebScriptObject *)windowScriptObject forFrame:(WebFrame *)frame 
{
  BowlineScriptDelegate* myScriptDelegate = [[BowlineScriptDelegate alloc] initWithWxWindow: webKitWindow forFrame: frame];
  [windowScriptObject setValue:myScriptDelegate forKey:@"_app"];
}
@end

@implementation BowlineScriptDelegate

- initWithWxWindow: (BowlineWebKit*)inWindow forFrame:(WebFrame *)inFrame
{
  [super init];
  webKitWindow = inWindow;
  frame = inFrame;
  return self;
}

- (void)call:(NSString *)data
{
  if (webKitWindow){
    wxWebKitScriptEvent thisEvent(webKitWindow);
    thisEvent.SetData( wxStringWithNSString(data) );
    if (webKitWindow->GetEventHandler())
        webKitWindow->GetEventHandler()->ProcessEvent( thisEvent );
  }
}

- (void)showInspector
{
  if (webKitWindow){
    webKitWindow->ShowInspector();
  }
}

+ (BOOL)isSelectorExcludedFromWebScript:(SEL)sel
{
  if (sel == @selector(call:)) return NO;
  if (sel == @selector(showInspector:)) return NO;
  return YES;
}

+ (NSString *) webScriptNameForSelector:(SEL)sel
{
  if (sel == @selector(call:)) return @"call";
  if (sel == @selector(showInspector:)) return @"showInspector";
  return nil;
}

+ (BOOL) isKeyExcludedFromWebScript:(const char*)key {
  return YES;
}

@end

@implementation BowlinePolicyDelegate

- initWithWxWindow: (BowlineWebKit*)inWindow
{
  [super init];
  webKitWindow = inWindow;
  return self;
}

- (void)webView:(WebView *)sender decidePolicyForNavigationAction:(NSDictionary *)actionInformation request:(NSURLRequest *)request frame:(WebFrame *)frame decisionListener:(id<WebPolicyDecisionListener>)listener
{
  [listener use];
}

- (void)webView:(WebView *)sender decidePolicyForNewWindowAction:(NSDictionary *)actionInformation request:(NSURLRequest *)request newFrameName:(NSString *)frameName decisionListener:(id < WebPolicyDecisionListener >)listener
{
  [listener use];
}
@end

@implementation BowlineUIDelegate

- initWithWxWindow: (BowlineWebKit*)inWindow
{
  [super init];
  webKitWindow = inWindow;
  return self;
}

- (NSArray *)webView:(WebView *)sender contextMenuItemsForElement:(NSDictionary *)element defaultMenuItems:(NSArray *)defaultMenuItems 
{
  NSMutableArray *webViewMenuItems = [[defaultMenuItems mutableCopy] autorelease];

  if (webViewMenuItems)
  {
    NSEnumerator *itemEnumerator = [defaultMenuItems objectEnumerator];
    NSMenuItem *menuItem = nil;
    while ((menuItem = [itemEnumerator nextObject]))
    {
      NSInteger tag = [menuItem tag];
      
      if ( !webKitWindow->EnabledDeveloper() ){
        switch (tag) {
          case WebMenuItemTagReload:
          case WebMenuItemTagInspectElement:
            [webViewMenuItems removeObjectIdenticalTo: menuItem];
        }
      }

      switch (tag)
      {
        case WebMenuItemTagOpenLinkInNewWindow:
        case WebMenuItemTagDownloadLinkToDisk:
        case WebMenuItemTagCopyLinkToClipboard:
        case WebMenuItemTagOpenImageInNewWindow:
        case WebMenuItemTagDownloadImageToDisk:
        case WebMenuItemTagCopyImageToClipboard:
        case WebMenuItemTagOpenFrameInNewWindow:
        case WebMenuItemTagGoBack:
        case WebMenuItemTagGoForward:
        case WebMenuItemTagStop:
        case WebMenuItemTagOpenWithDefaultApplication:
          [webViewMenuItems removeObjectIdenticalTo: menuItem];
      }
    }
  }

  return webViewMenuItems;
}

- (void)webView:(WebView *)sender runJavaScriptAlertPanelWithMessage:(NSString *)message initiatedByFrame:(WebFrame *)frame
{
  wxMessageBox(wxStringWithNSString(message), wxT("JavaScript Alert"), wxOK);
}

- (BOOL)webView:(WebView *)sender runJavaScriptConfirmPanelWithMessage:(NSString *)message initiatedByFrame:(WebFrame *)frame
{
  wxMessageDialog dialog(NULL, wxStringWithNSString(message), wxT("JavaScript Confirm"), wxYES_NO);
  dialog.Centre();
  return(dialog.ShowModal() == wxID_YES);
}

- (NSString *)webView:(WebView *)sender runJavaScriptTextInputPanelWithPrompt:(NSString *)prompt defaultText:(NSString *)defaultText initiatedByFrame:(WebFrame *)frame
{
  wxTextEntryDialog dialog(NULL, wxStringWithNSString(prompt), wxT("JavaScript Prompt"), wxEmptyString, wxOK | wxCANCEL);
  dialog.Centre();
  if (dialog.ShowModal() == wxID_OK) {
    return wxNSStringWithWxString((wxString) dialog.GetValue());
  }
  return nil;
}

- (WebView *)webView:(WebView *)sender createWebViewWithRequest:(NSURLRequest *)request windowFeatures:(NSDictionary *)features
{
  SimpleWindow* window = new SimpleWindow;
  
  NSString* url = [[request URL] absoluteString];
  window->LoadURL(wxStringWithNSString(url));
  
  int x = [[features objectForKey:@"x"] intValue];
  int y = [[features objectForKey:@"y"] intValue];
  int width = [[features objectForKey:@"width"] intValue];
  int height = [[features objectForKey:@"height"] intValue];
  
  window->SetSize(x, y, width, height, wxSIZE_USE_EXISTING);
  window->Show();

  return window->webkit->m_webView;
}

- (void)webViewShow:(WebView *)sender
{
  webKitWindow->Show();
}

- (void)webViewClose:(WebView *)wv 
{
  webKitWindow->Close(false);
}

- (void)webViewFocus:(WebView *)wv 
{
  webKitWindow->SetFocus();
}

@end
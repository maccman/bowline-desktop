#include "bowline_webkit.cpp"

bool BowlineWebKit::Create(wxWindow *parent,
                           wxWindowID winID,
                           const wxPoint& pos,
                           const wxSize& size, long style,
                           const wxValidator& validator,
                           const wxString& name){
  Init();

  if ( !CreateControl(parent, id, pos, size, style, validator, name) )
     return false;
     
                             
  
  HRESULT hr = WebKitCreateInstance(CLSID_WebView, 0, IID_IWebView, (void**) &(this->webView));
 	if (FAILED(hr))
 		HandleHResultError("Error creating WebKitWebView", hr, true);
 	// TODO
 	
 	webView->setApplicationNameForUserAgent(ua.copy());
 	
 	IWebPreferences *prefs = NULL;
	hr = WebKitCreateInstance(CLSID_WebPreferences, 0, IID_IWebPreferences,
		(void**) &prefs);
	if (FAILED(hr) || !prefs)
		HandleHResultError("Error getting IWebPreferences", hr, true);
		
	prefs->initWithIdentifier(pi.copy(), &prefs);
	prefs->setCacheModel(WebCacheModelDocumentBrowser);
	prefs->setPlugInsEnabled(true);
	prefs->setJavaEnabled(true);
	prefs->setJavaScriptEnabled(true);
	prefs->setJavaScriptCanOpenWindowsAutomatically(false);
	prefs->setDOMPasteAllowed(true);
	
	IWebPreferencesPrivate* privatePrefs = NULL;
	hr = prefs->QueryInterface(IID_IWebPreferencesPrivate, (void**) &privatePrefs);
	if (FAILED(hr) || !privatePrefs)
		HandleHResultError("Error getting IWebPreferencesPrivate", hr, true);

	privatePrefs->setDeveloperExtrasEnabled(false);
	privatePrefs->setDatabasesEnabled(true);
	privatePrefs->setLocalStorageEnabled(true);
	privatePrefs->setOfflineWebApplicationCacheEnabled(true);
	privatePrefs->setAllowUniversalAccessFromFileURLs(true);
	
}

BowlineWebKit::~BowlineWebKit() { 
}

void BowlineWebKit::SetPageSource(const wxString& source, const wxString& baseUrl){
}

wxString BowlineWebKit::GetPageSource(){
  return wxEmptyString;
}

void BowlineWebKit::LoadURL(const wxString& url){
  std::string url = URLUtils::NormalizeURL(url_);
	Win32UIBinding::SetProxyForURL(url);

	IWebMutableURLRequest* request = 0;
	std::wstring method = L"GET" ;

	HRESULT hr = WebKitCreateInstance(CLSID_WebMutableURLRequest, 0, 
		IID_IWebMutableURLRequest, (void**) &request);
	if (FAILED(hr))
		HandleHResultError("Error creating WebMutableURLRequest", hr, true);

	std::wstring wurl = ::UTF8ToWide(url);
	hr = request->initWithURL(SysAllocString(wurl.c_str()), 
		WebURLRequestUseProtocolCachePolicy, 60);
	if (FAILED(hr))
	{
		request->Release();
		std::string error("Error initialiazing WebMutableURLRequest for ");
		error.append(url);
		HandleHResultError(error, hr, true);
	}

	hr = request->setHTTPMethod(SysAllocString(method.c_str()));
	if (FAILED(hr))
	{
		request->Release();
		std::string error("Error setting HTTP method for ");
		error.append(url);
		HandleHResultError(error, hr, true);
	}

	hr = mainFrame->loadRequest(request);
	if (FAILED(hr))
	{
		request->Release();
		std::string error("Error starting load request for ");
		error.append(url);
		HandleHResultError(error, hr, true);
	}
}

wxString BowlineWebKit::RunScript(const wxString& javascript){
  return wxEmptyString;
}

void wxWebKitCtrl::ShowInspector(bool console){
  // TODO - inspector on Windows
}

void BowlineWebKit::Cut(){
}

void BowlineWebKit::Copy(){
}

void BowlineWebKit::Paste(){
}

void BowlineWebKit::Reload(){
}
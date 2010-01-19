#include "bowline_webkit.cpp"

bool BowlineWebKit::Create(wxWindow *parent,
                           wxWindowID winID,
                           const wxPoint& pos,
                           const wxSize& size, long style,
                           const wxValidator& validator,
                           const wxString& name){
  HRESULT hr = WebKitCreateInstance(CLSID_WebView, 0, IID_IWebView, (void**) &(this->webView));
 	if (FAILED(hr))
 		HandleHResultError("Error creating WebKitWebView", hr, true);
 	// TODO
}

BowlineWebKit::~BowlineWebKit() { 
}

void BowlineWebKit::SetPageSource(const wxString& source){
}

wxString BowlineWebKit::GetPageSource(){
  return wxEmptyString;
}

void BowlineWebKit::LoadURL(const wxString& url){
}

wxString BowlineWebKit::RunScript(const wxString& javascript){
  return wxEmptyString;
}

void wxWebKitCtrl::ShowInspector(bool console){
}
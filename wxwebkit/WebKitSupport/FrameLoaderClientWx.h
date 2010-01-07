#ifndef FRAMELOADERCLIENTWX_H_DDG8YT4I
#define FRAMELOADERCLIENTWX_H_DDG8YT4I

#include "EmptyClients.h"

class wxWebFrame;
class wxWebView;

namespace WebCore {

class FrameLoaderClientWx : public EmptyFrameLoaderClient
{
public:
  FrameLoaderClientWx();
  virtual ~FrameLoaderClientWx();
  
  void setFrame(wxWebFrame *frame);
  void setWebView(wxWebView *webview);
  bool hasWebView() const;

private:
  wxWebFrame *m_webFrame;
  Frame* m_frame;
  wxWebView *m_webView;
};

} // WebCore
#endif /* end of include guard: FRAMELOADERCLIENTWX_H_DDG8YT4I */

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
  virtual bool hasWebView() const;
  virtual void transitionToCommittedForNewPage();
  
  virtual void dispatchDidStartProvisionalLoad();
  virtual void dispatchDidCommitLoad();
  virtual void dispatchDidFinishDocumentLoad();
  virtual void postProgressFinishedNotification();
  virtual void dispatchDidFailLoading(DocumentLoader*, unsigned long, const ResourceError&);
  virtual void dispatchWillSubmitForm(FramePolicyFunction, PassRefPtr<FormState>);
  virtual void dispatchDecidePolicyForMIMEType(FramePolicyFunction function, const String&, const ResourceRequest&);
  virtual void dispatchDecidePolicyForNewWindowAction(FramePolicyFunction function, const NavigationAction&, const ResourceRequest&, PassRefPtr<FormState>, const String&);
  virtual void dispatchDecidePolicyForNavigationAction(FramePolicyFunction, const NavigationAction&, const ResourceRequest&, PassRefPtr<FormState>);
  virtual void dispatchDidClearWindowObjectInWorld(DOMWrapperWorld*);
  
  virtual PassRefPtr<Frame> createFrame(const KURL& url, const String& name, HTMLFrameOwnerElement* ownerElement,
                             const String& referrer, bool allowsScrolling, int marginWidth, int marginHeight);
  virtual void committedLoad(DocumentLoader*, const char*, int);
  
  
  virtual String userAgent(const KURL&);
  virtual bool canShowMIMEType(const String&) const;
  virtual bool canHandleRequest(const ResourceRequest&) const;

private:
  wxWebFrame *m_webFrame;
  Frame* m_frame;
  wxWebView *m_webView;
};

} // WebCore
#endif /* end of include guard: FRAMELOADERCLIENTWX_H_DDG8YT4I */

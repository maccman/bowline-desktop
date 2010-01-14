#include "config.h"
#include "FrameLoaderClientWx.h"

#include "JavaScript.h"
#include "APICast.h"
#include "MIMETypeRegistry.h"

#include "WebString.h"
#include "WebRect.h"
#include "WebFrame.h"
#include "WebFramePrivate.h"
#include "WebView.h"
#include "WebViewPrivate.h"

#include <iostream>

namespace WebCore {

FrameLoaderClientWx::FrameLoaderClientWx()
    : m_frame(0)
    , m_webFrame(0)
{
}

FrameLoaderClientWx::~FrameLoaderClientWx()
{
}

void FrameLoaderClientWx::setFrame(wxWebFrame *frame)
{
    m_webFrame = frame;
    m_frame = m_webFrame->m_impl->frame;
}

void FrameLoaderClientWx::setWebView(wxWebView *webview)
{
    m_webView = webview;
}

bool FrameLoaderClientWx::hasWebView() const
{
    return m_webView != NULL;
}

void FrameLoaderClientWx::transitionToCommittedForNewPage()
{ 
    ASSERT(m_webFrame);
    ASSERT(m_frame);
    ASSERT(m_webView);
    
    IntSize size = IntRect((WebRect) m_webView->GetRect()).size();
    // FIXME: This value should be gotten from m_webView->IsTransparent();
    // but transitionToCommittedForNewPage() can be called while m_webView is
    // still being initialized.
    bool transparent = false;
    Color backgroundColor = transparent ? WebCore::Color::transparent : WebCore::Color::white;
    
    if (m_frame)
        m_frame->createView(size, backgroundColor, transparent, IntSize(), false); 
}

void FrameLoaderClientWx::dispatchDidStartProvisionalLoad()
{
    if (m_webView) {
        wxWebViewLoadEvent wkEvent(m_webView);
        wkEvent.SetState(wxWEBVIEW_LOAD_NEGOTIATING);
        wkEvent.SetURL((WebString)m_frame->loader()->provisionalDocumentLoader()->request().url().string());
        m_webView->GetEventHandler()->ProcessEvent(wkEvent);
    }
}

void FrameLoaderClientWx::dispatchDidCommitLoad()
{
    if (m_webView) {
        wxWebViewLoadEvent wkEvent(m_webView);
        wkEvent.SetState(wxWEBVIEW_LOAD_TRANSFERRING);
        wkEvent.SetURL((WebString)m_frame->loader()->documentLoader()->request().url().string());
        m_webView->GetEventHandler()->ProcessEvent(wkEvent);
    }
}

void FrameLoaderClientWx::dispatchDidFinishDocumentLoad()
{
    if (m_webView) {
        wxWebViewLoadEvent wkEvent(m_webView);
        wkEvent.SetState(wxWEBVIEW_LOAD_DOC_COMPLETED);
        wkEvent.SetURL((WebString)m_frame->loader()->url().string());
        m_webView->GetEventHandler()->ProcessEvent(wkEvent);
    }
}

void FrameLoaderClientWx::postProgressFinishedNotification()
{
    if (m_webView) {
        wxWebViewLoadEvent wkEvent(m_webView);
        wkEvent.SetState(wxWEBVIEW_LOAD_DL_COMPLETED);
        wkEvent.SetURL((WebString)m_frame->loader()->url().string());
        m_webView->GetEventHandler()->ProcessEvent(wkEvent);
    }
}

void FrameLoaderClientWx::dispatchDidFailLoading(DocumentLoader*, unsigned long, const ResourceError&)
{
    if (m_webView) {
        wxWebViewLoadEvent wkEvent(m_webView);
        wkEvent.SetState(wxWEBVIEW_LOAD_FAILED);
        wkEvent.SetURL((WebString)m_frame->loader()->documentLoader()->request().url().string());
        m_webView->GetEventHandler()->ProcessEvent(wkEvent);
    }
}

void FrameLoaderClientWx::dispatchWillSubmitForm(FramePolicyFunction function,
                                                 PassRefPtr<FormState>)
{
    if (!m_webFrame)
        return;
    (m_frame->loader()->policyChecker()->*function)(PolicyUse);
}

void FrameLoaderClientWx::dispatchDecidePolicyForMIMEType(FramePolicyFunction function, const String& mimetype, const ResourceRequest& request)
{
    if (!m_webFrame)
        return;    
    (m_frame->loader()->policyChecker()->*function)(PolicyUse);
}

void FrameLoaderClientWx::dispatchDecidePolicyForNewWindowAction(FramePolicyFunction function, const NavigationAction&, const ResourceRequest& request, PassRefPtr<FormState>, const String& targetName)
{
}

void FrameLoaderClientWx::dispatchDecidePolicyForNavigationAction(FramePolicyFunction function, const NavigationAction& action, const ResourceRequest& request, PassRefPtr<FormState>)
{
  if (!m_webFrame)
      return;
  (m_frame->loader()->policyChecker()->*function)(PolicyUse);
}

PassRefPtr<Frame> FrameLoaderClientWx::createFrame(const KURL& url, const String& name, HTMLFrameOwnerElement* ownerElement,
                                   const String& referrer, bool allowsScrolling, int marginWidth, int marginHeight)
{
}

void FrameLoaderClientWx::dispatchDidClearWindowObjectInWorld(DOMWrapperWorld* world)
{
    if (world != mainThreadNormalWorld())
        return;

    if (m_webView) {
        wxWebViewWindowObjectClearedEvent wkEvent(m_webView);
        Frame* coreFrame = m_webView->GetMainFrame()->GetFrame();
        JSGlobalContextRef context = toGlobalRef(coreFrame->script()->globalObject(mainThreadNormalWorld())->globalExec());
        JSObjectRef windowObject = toRef(coreFrame->script()->globalObject(mainThreadNormalWorld()));
        wkEvent.SetJSContext(context);
        wkEvent.SetWindowObject(windowObject);
        m_webView->GetEventHandler()->ProcessEvent(wkEvent);
    }
}

void FrameLoaderClientWx::committedLoad(WebCore::DocumentLoader* loader, const char* data, int length)
{
}

String FrameLoaderClientWx::userAgent(const KURL&)
{
    return String("Mozilla/5.0 (Macintosh; U; Intel Mac OS X; en) AppleWebKit/418.9.1 (KHTML, like Gecko) Safari/419.3 (Bowline)");
}

bool FrameLoaderClientWx::canShowMIMEType(const String& type) const
{
    return (MIMETypeRegistry::isSupportedImageMIMEType(type)
            || MIMETypeRegistry::isSupportedNonImageMIMEType(type)
            || MIMETypeRegistry::isSupportedMediaMIMEType(type));
}

bool FrameLoaderClientWx::canHandleRequest(const ResourceRequest&) const
{
    return true;
}

}
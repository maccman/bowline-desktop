#ifndef EMPTYFRAMELOADERCLIENTWX_H_PR3JFFC7
#define EMPTYFRAMELOADERCLIENTWX_H_PR3JFFC7

#include "config.h"

#include "FrameLoaderClient.h"
#include "DocumentLoader.h"
#include "FormState.h"
#include "Frame.h"
#include "FrameLoaderTypes.h"
#include "FrameView.h"
#include "FrameTree.h"
#include "PluginView.h"
#include "HTMLFormElement.h"
#include "HTMLFrameOwnerElement.h"
#include "NotImplemented.h"
#include "Page.h"
#include "PlatformString.h"
#include "PluginView.h"
#include "ProgressTracker.h"
#include "RenderPart.h"
#include "ResourceError.h"
#include "ResourceResponse.h"
#include "ScriptController.h"
#include "ScriptString.h"
#include <wtf/PassRefPtr.h>
#include <wtf/RefPtr.h>

namespace WebCore {

class EmptyFrameLoaderClientWx : public FrameLoaderClient {
public:  
    EmptyFrameLoaderClientWx() { }
    virtual ~EmptyFrameLoaderClientWx() { }
    
    virtual void frameLoaderDestroyed() { }

    virtual bool hasHTMLView() const { return true; }
    virtual bool hasWebView() const { return true; } // mainly for assertions

    virtual void makeRepresentation(DocumentLoader*) { }
    virtual void forceLayout() { }
    virtual void forceLayoutForNonHTML() { }

    virtual void setCopiesOnScroll() { }

    virtual void detachedFromParent2() { }
    virtual void detachedFromParent3() { }

    virtual void assignIdentifierToInitialRequest(unsigned long, DocumentLoader*, const ResourceRequest&) { }
    
    virtual void dispatchWillSendRequest(DocumentLoader*, unsigned long, ResourceRequest&, const ResourceResponse&) { }
    virtual bool shouldUseCredentialStorage(DocumentLoader*, unsigned long) { return false; }
    virtual void dispatchDidReceiveAuthenticationChallenge(DocumentLoader*, unsigned long, const AuthenticationChallenge&) { }
    virtual void dispatchDidCancelAuthenticationChallenge(DocumentLoader*, unsigned long, const AuthenticationChallenge&) { }
    virtual void dispatchDidReceiveResponse(DocumentLoader*, unsigned long, const ResourceResponse&) { }
    virtual void dispatchDidReceiveContentLength(DocumentLoader*, unsigned long, int) { }
    virtual void dispatchDidFinishLoading(DocumentLoader*, unsigned long) { }
    virtual void dispatchDidFailLoading(DocumentLoader*, unsigned long, const ResourceError&) { }
    virtual bool dispatchDidLoadResourceFromMemoryCache(DocumentLoader*, const ResourceRequest&, const ResourceResponse&, int) { return false; }
    virtual void dispatchDidLoadResourceByXMLHttpRequest(unsigned long, const ScriptString&) { }

    virtual void dispatchDidHandleOnloadEvents() { }
    virtual void dispatchDidReceiveServerRedirectForProvisionalLoad() { }
    virtual void dispatchDidCancelClientRedirect() { }
    virtual void dispatchWillPerformClientRedirect(const KURL&, double, double) { }
    virtual void dispatchDidChangeLocationWithinPage() { }
    virtual void dispatchDidPushStateWithinPage() { }
    virtual void dispatchDidReplaceStateWithinPage() { }
    virtual void dispatchDidPopStateWithinPage() { }
    virtual void dispatchWillClose() { }
    virtual void dispatchDidReceiveIcon() { }
    virtual void dispatchDidStartProvisionalLoad() { }
    virtual void dispatchDidReceiveTitle(const String&) { }
    virtual void dispatchDidCommitLoad() { }
    virtual void dispatchDidFailProvisionalLoad(const ResourceError&) { }
    virtual void dispatchDidFailLoad(const ResourceError&) { }
    virtual void dispatchDidFinishDocumentLoad() { }
    virtual void dispatchDidFinishLoad() { }
    virtual void dispatchDidFirstLayout() { }
    virtual void dispatchDidFirstVisuallyNonEmptyLayout() { }

    virtual Frame* dispatchCreatePage() { return 0; }
    virtual void dispatchShow() { }

    virtual void dispatchDecidePolicyForMIMEType(FramePolicyFunction, const String&, const ResourceRequest&) { }
    virtual void dispatchDecidePolicyForNewWindowAction(FramePolicyFunction, const NavigationAction&, const ResourceRequest&, PassRefPtr<FormState>, const String&) { }
    virtual void dispatchDecidePolicyForNavigationAction(FramePolicyFunction, const NavigationAction&, const ResourceRequest&, PassRefPtr<FormState>) { }
    virtual void cancelPolicyCheck() { }

    virtual void dispatchUnableToImplementPolicy(const ResourceError&) { }

    virtual void dispatchWillSubmitForm(FramePolicyFunction, PassRefPtr<FormState>) { }

    virtual void dispatchDidLoadMainResource(DocumentLoader*) { }
    virtual void revertToProvisionalState(DocumentLoader*) { }
    virtual void setMainDocumentError(DocumentLoader*, const ResourceError&) { }

    virtual void willChangeEstimatedProgress() { }
    virtual void didChangeEstimatedProgress() { }
    virtual void postProgressStartedNotification() { }
    virtual void postProgressEstimateChangedNotification() { }
    virtual void postProgressFinishedNotification() { }

    virtual void setMainFrameDocumentReady(bool) { }

    virtual void startDownload(const ResourceRequest&) { }

    virtual void willChangeTitle(DocumentLoader*) { }
    virtual void didChangeTitle(DocumentLoader*) { }

    virtual void committedLoad(DocumentLoader*, const char*, int) { }
    virtual void finishedLoading(DocumentLoader*) { }

    virtual void updateGlobalHistory() { }
    virtual void updateGlobalHistoryRedirectLinks() { }
    
    virtual bool shouldGoToHistoryItem(HistoryItem*) const { return false; }
    virtual void dispatchDidAddBackForwardItem(HistoryItem*) const { }
    virtual void dispatchDidRemoveBackForwardItem(HistoryItem*) const { };
    virtual void dispatchDidChangeBackForwardIndex() const { }
    
    virtual void didDisplayInsecureContent() { }
    
    virtual void didRunInsecureContent(SecurityOrigin*) { }
    
    virtual ResourceError cancelledError(const ResourceRequest&) { ResourceError error("", 0, "", ""); error.setIsCancellation(true); return error; }
    virtual ResourceError blockedError(const ResourceRequest&) { return ResourceError("", 0, "", ""); }
    virtual ResourceError cannotShowURLError(const ResourceRequest&) { return ResourceError("", 0, "", ""); }
    virtual ResourceError interruptForPolicyChangeError(const ResourceRequest&) { return ResourceError("", 0, "", ""); }
    
    virtual ResourceError cannotShowMIMETypeError(const ResourceResponse&) { return ResourceError("", 0, "", ""); }
    virtual ResourceError fileDoesNotExistError(const ResourceResponse&) { return ResourceError("", 0, "", ""); }
    virtual ResourceError pluginWillHandleLoadError(const ResourceResponse&) { return ResourceError("", 0, "", ""); }
    
    virtual bool shouldFallBack(const ResourceError&) { return false; }
    
    virtual bool canHandleRequest(const ResourceRequest&) const { return false; }
    virtual bool canShowMIMEType(const String&) const { return false; }
    virtual bool representationExistsForURLScheme(const String&) const { return false; }
    virtual String generatedMIMETypeForURLScheme(const String&) const { return ""; }
    
    virtual void frameLoadCompleted() { }
    virtual void saveViewStateToItem(HistoryItem*) { }
    virtual void restoreViewState() { }
    virtual void provisionalLoadStarted() { }
    virtual void didFinishLoad() { }
    virtual void prepareForDataSourceReplacement() { }
    
    virtual PassRefPtr<DocumentLoader> createDocumentLoader(const ResourceRequest& request, const SubstituteData& substituteData) { return DocumentLoader::create(request, substituteData); }
    virtual void setTitle(const String&, const KURL&) { }
    
    virtual String userAgent(const KURL&) { return ""; }
    
    virtual void savePlatformDataToCachedFrame(CachedFrame*) { }
    virtual void transitionToCommittedFromCachedFrame(CachedFrame*) { }
    virtual void transitionToCommittedForNewPage() { }

    virtual bool canCachePage() const { return false; }    
    virtual void download(ResourceHandle*, const ResourceRequest&, const ResourceRequest&, const ResourceResponse&) { }
    
    virtual PassRefPtr<Frame> createFrame(const KURL&, const String&, HTMLFrameOwnerElement*, const String&, bool, int, int) { return 0; }
    virtual PassRefPtr<Widget> createPlugin(const IntSize&, HTMLPlugInElement*, const KURL&, const Vector<String>&, const Vector<String>&, const String&, bool) { return 0; }
    virtual void redirectDataToPlugin(Widget*) { }
    
    virtual PassRefPtr<Widget> createJavaAppletWidget(const IntSize&, HTMLAppletElement*, const KURL&, const Vector<String>&, const Vector<String>&) { return 0; }
    
    virtual void dispatchDidFailToStartPlugin(const PluginView*) const { }
    
    virtual ObjectContentType objectContentType(const KURL&, const String&) { return ObjectContentType(); }
    virtual String overrideMediaType() const { return String(); }
    
    virtual void dispatchDidClearWindowObjectInWorld(DOMWrapperWorld*) { }
    virtual void documentElementAvailable() { }
    virtual void didPerformFirstNavigation() const { }
    
#if USE(V8)
    virtual void didCreateScriptContextForFrame() { }
    virtual void didDestroyScriptContextForFrame() { }
    virtual void didCreateIsolatedScriptContext() { }
#endif

    virtual void registerForIconNotification(bool) { }

#if PLATFORM(MAC)
#if ENABLE(MAC_JAVA_BRIDGE)
      virtual jobject javaApplet(NSView*) { return 0; }
#endif
    virtual NSCachedURLResponse* willCacheResponse(DocumentLoader*, unsigned long, NSCachedURLResponse* response) const { return response; }
#endif
#if USE(CFNETWORK)
    virtual bool shouldCacheResponse(DocumentLoader*, unsigned long, const ResourceResponse&, const unsigned char*, unsigned long long) { return true; }
#endif    
};

}

#endif /* end of include guard: EMPTYFRAMELOADERCLIENTWX_H_PR3JFFC7 */
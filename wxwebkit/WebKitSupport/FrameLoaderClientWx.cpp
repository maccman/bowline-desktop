#include "config.h"
#include "FrameLoaderClientWx.h"

#include "WebString.h"
#include "WebFrame.h"
#include "WebFramePrivate.h"
#include "WebView.h"
#include "WebViewPrivate.h"

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

}
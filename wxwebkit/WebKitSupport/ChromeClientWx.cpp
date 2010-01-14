/*
 * Copyright (C) 2007 Kevin Ollivier <kevino@theolliviers.com>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "ChromeClientWx.h"
#include "Console.h"
#if ENABLE(DATABASE)
#include "DatabaseTracker.h"
#endif
#include "FileChooser.h"
#include "FloatRect.h"
#include "Frame.h"
#include "FrameLoadRequest.h"
#include "NotImplemented.h"
#include "PlatformString.h"

#include <stdio.h>
#include <iostream>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/textdlg.h>
#include <wx/tooltip.h>

#include "WebString.h"
#include "WebRect.h"
#include "WebView.h"
#include "WebViewPrivate.h"

namespace WebCore {

ChromeClientWx::ChromeClientWx(wxWebView* webView)
{
    m_webView = webView;
}

void ChromeClientWx::chromeDestroyed(){
  // delete this;
}

// Page* ChromeClientWx::createWindow(Frame*, const FrameLoadRequest& request, const WindowFeatures&)
// {
// 
//     // FIXME: Create a EVT_WEBKIT_NEW_WINDOW event, and only run this code
//     // when that event is not handled.
//     
//     Page* myPage = 0;
//     wxWebBrowserShell* newFrame = new wxWebBrowserShell(wxTheApp->GetAppName());
//     
//     if (newFrame->webview) {
//         newFrame->webview->LoadURL(request.resourceRequest().url().string());
//         newFrame->Show(true);
// 
//         WebViewPrivate* impl = newFrame->webview->m_impl;
//         if (impl)
//             myPage = impl->page;
//     }
//     
//     return myPage;
// }

void ChromeClientWx::addMessageToConsole(MessageSource source,
                                          MessageType type,
                                          MessageLevel level,
                                          const String& message,
                                          unsigned int lineNumber,
                                          const String& sourceID)
{
    if (m_webView) {
        // wxWebViewConsoleMessageEvent wkEvent(m_webView);
        // wkEvent.SetMessage((WebString)message);
        // wkEvent.SetLineNumber(WebString)lineNumber);
        // wkEvent.SetSourceID(sourceID);
        // wkEvent.SetLevel(static_cast<wxWebViewConsoleMessageLevel>(level));
        // m_webView->GetEventHandler()->ProcessEvent(wkEvent);
    }
}

bool ChromeClientWx::canRunBeforeUnloadConfirmPanel(){
  return true;
}

bool ChromeClientWx::runBeforeUnloadConfirmPanel(const String& string,
                                                  Frame* frame)
{
    wxMessageDialog dialog(NULL, (WebString)string, wxT("Confirm Action?"), wxYES_NO);
    return dialog.ShowModal() == wxYES;
}

void ChromeClientWx::runJavaScriptAlert(Frame* frame, const String& string)
{
    if (m_webView) {
        wxWebViewAlertEvent wkEvent(m_webView);
        wkEvent.SetMessage((WebString)string);
        if (!m_webView->GetEventHandler()->ProcessEvent(wkEvent))
            wxMessageBox((WebString)string, wxT("JavaScript Alert"), wxOK);
    }
}

bool ChromeClientWx::runJavaScriptConfirm(Frame* frame, const String& string)
{
    bool result = false;
    if (m_webView) {
        wxWebViewConfirmEvent wkEvent(m_webView);
        wkEvent.SetMessage((WebString)string);
        if (m_webView->GetEventHandler()->ProcessEvent(wkEvent))
            result = wkEvent.GetReturnCode() == wxID_YES;
        else {
            wxMessageDialog dialog(NULL, (WebString)string, wxT("JavaScript Confirm"), wxYES_NO);
            dialog.Centre();
            result = (dialog.ShowModal() == wxID_YES);
        }
    }
    return result;
}

bool ChromeClientWx::runJavaScriptPrompt(Frame* frame, const String& message, const String& defaultValue, String& result)
{
    if (m_webView) {
        wxWebViewPromptEvent wkEvent(m_webView);
        wkEvent.SetMessage((WebString)message);
        wkEvent.SetResponse((WebString)defaultValue);
        if (m_webView->GetEventHandler()->ProcessEvent(wkEvent)) {
            result = (WebString)wkEvent.GetResponse();
            return true;
        }
        else {
            wxTextEntryDialog dialog(NULL, (WebString)message, wxT("JavaScript Prompt"), wxEmptyString, wxOK | wxCANCEL);
            dialog.Centre();
            if (dialog.ShowModal() == wxID_OK) {
                result = (WebString)dialog.GetValue();
                return true;
            }
        }
    }
    return false;
}

void ChromeClientWx::repaint(const IntRect& rect, bool contentChanged, bool immediate, bool repaintContentOnly)
{
    if (!m_webView)
        return;
    
    if (contentChanged)
        m_webView->RefreshRect((WebRect)rect);
    
    if (immediate) {
        m_webView->Update();
    }
}

PlatformPageClient ChromeClientWx::platformPageClient() const
{
  std::cout << "Called ChromeClientWx::platformPageClient()\n";
  return 0;
    // return m_webView;
}

void ChromeClientWx::setToolTip(const String& tip, TextDirection)
{
    wxToolTip* tooltip = m_webView->GetToolTip();
    if (!tooltip || tooltip->GetTip() != wxString((WebString)tip))
        m_webView->SetToolTip((WebString)tip);
}

}

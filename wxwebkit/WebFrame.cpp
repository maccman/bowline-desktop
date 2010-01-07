/*
 * Copyright (C) 2007 Kevin Ollivier  All rights reserved.
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
#include "BackForwardList.h"
#include "CString.h"
#include "Document.h"
#include "Editor.h"
#include "Element.h"
#include "EventHandler.h"
#include "Frame.h"
#include "FrameLoader.h"
#include "FrameView.h"
#include "HitTestResult.h"
#include "HostWindow.h"
#include "HTMLFrameOwnerElement.h"
#include "markup.h"
#include "Page.h"
#include "PlatformString.h"
#include "RenderTreeAsText.h"
#include "RenderObject.h"
#include "RenderView.h"
#include "ScriptController.h"
#include "ScriptValue.h"
#include "SubstituteData.h"
#include "TextEncoding.h"

#include "JSDOMBinding.h"
#include <runtime/JSValue.h>
#include <runtime/UString.h>

// #include "EditorClientWx.h"
#include "FrameLoaderClientWx.h"

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "WebFrame.h"
#include "WebView.h"
#include "WebFramePrivate.h"
#include "WebViewPrivate.h"

#include <wx/defs.h>
#include <wx/dcbuffer.h>

wxWebFrame::wxWebFrame(wxWebView* container, wxWebFrame* parent, WebViewFrameData* data) :
    m_isInitialized(false),
    m_beingDestroyed(false)
{

    m_impl = new WebFramePrivate();
 
    WebCore::HTMLFrameOwnerElement* parentFrame = 0;
    
    if (data) {
        parentFrame = data->ownerElement;
    }
    
    WebCore::FrameLoaderClientWx* loaderClient = new WebCore::FrameLoaderClientWx();
    RefPtr<WebCore::Frame> newFrame = WebCore::Frame::create(container->m_impl->page, parentFrame, loaderClient);

    m_impl->frame = newFrame.get();

    loaderClient->setFrame(this);
    loaderClient->setWebView(container);
    
    if (data && data->ownerElement)
        m_impl->frame->ref();

    m_impl->frame->init();
        
    m_isInitialized = true;
}

wxWebFrame::~wxWebFrame()
{
    if (m_impl)
        delete m_impl;
}

WebCore::Frame* wxWebFrame::GetFrame()
{
    if (m_impl)
        return m_impl->frame;
        
    return 0;
}

void wxWebFrame::Reload()
{
    if (m_impl->frame && m_impl->frame->loader())
        m_impl->frame->loader()->reload();
}

wxString wxWebFrame::RunScript(const wxString& javascript)
{
    wxString returnValue = wxEmptyString;
    // TODO - conversion
    // if (m_impl->frame && m_impl->frame->loader()) {
    //     bool hasLoaded = m_impl->frame->loader()->frameHasLoaded();
    //     wxASSERT_MSG(hasLoaded, wxT("Document must be loaded before calling RunScript."));
    //     if (hasLoaded) {
    //         WebCore::ScriptController* controller = m_impl->frame->script();
    //         bool jsEnabled = controller->isEnabled(); 
    //         wxASSERT_MSG(jsEnabled, wxT("RunScript requires JavaScript to be enabled."));
    //         if (jsEnabled) {
    //             JSC::JSValue result = controller->executeScript(javascript, true).jsValue();
    //             if (result)
    //                 returnValue = wxString(result.toString(m_impl->frame->script()->globalObject(WebCore::mainThreadNormalWorld())->globalExec()).UTF8String().c_str(), wxConvUTF8);        
    //         }
    //     }
    // }
    return returnValue;
}

void wxWebFrame::LoadURL(const wxString& url)
{
    if (m_impl->frame && m_impl->frame->loader()) {
        // TODO - remove CURL
        // WebCore::KURL kurl = WebCore::KURL(WebCore::KURL(), url, WebCore::UTF8Encoding());
        // // NB: This is an ugly fix, but CURL won't load sub-resources if the
        // // protocol is omitted; sadly, it will not emit an error, either, so
        // // there's no way for us to catch this problem the correct way yet.
        // if (kurl.protocol().isEmpty()) {
        //     // is it a file on disk?
        //     if (wxFileExists(url)) {
        //         kurl.setProtocol("file");
        //         kurl.setPath("//" + kurl.path());
        //     }
        //     else {
        //         kurl.setProtocol("http");
        //         kurl.setPath("//" + kurl.path());
        //     }
        // }
        // m_impl->frame->loader()->load(kurl, false);
    }
}

bool wxWebFrame::ShouldClose() const
{
    if (m_impl->frame)
        return m_impl->frame->shouldClose();

    return true;
}
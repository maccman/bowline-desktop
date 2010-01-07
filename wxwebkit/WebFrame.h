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
 
#ifndef WXWEBFRAME_H
#define WXWEBFRAME_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "WebKitDefines.h"

class WebFramePrivate;
class WebViewFrameData;
class wxWebView;

namespace WebCore {
    class ChromeClientWx;
    class FrameLoaderClientWx;
    class EditorClientWx;
    class Frame;
}

class WXDLLIMPEXP_WEBKIT wxWebFrame
{
    // ChromeClientWx needs to get the Page* stored by the wxWebView
    // for the createWindow function. 
    friend class WebCore::ChromeClientWx;
    friend class WebCore::FrameLoaderClientWx;
    friend class WebCore::EditorClientWx;
    friend class wxWebView;

public:
    wxWebFrame(wxWebView* container, wxWebFrame* parent = NULL, WebViewFrameData* data = NULL);
    
    ~wxWebFrame();
    
    void LoadURL(const wxString& url);
    void Reload();
        
    wxString RunScript(const wxString& javascript);
        
    WebCore::Frame* GetFrame();
    
    bool ShouldClose() const;
    
private:
    bool m_isInitialized;
    bool m_beingDestroyed;
    WebFramePrivate* m_impl;    
};

#endif // ifndef WXWEBFRAME_H
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

#ifndef ChromeClientWx_H
#define ChromeClientWx_H

#include "EmptyClients.h"
#include "FocusDirection.h"
#include "IntRect.h"
#include "WebView.h"

namespace WebCore {

class ChromeClientWx : public EmptyChromeClient {
public:
    ChromeClientWx(wxWebView*);
    virtual ~ChromeClientWx() { };
    virtual void chromeDestroyed();

    virtual void addMessageToConsole(MessageSource source,
                                     MessageType type,
                                     MessageLevel level,
                                     const String& message,
                                     unsigned int lineNumber,
                                     const String& sourceID);

    virtual bool canRunBeforeUnloadConfirmPanel();
    virtual bool runBeforeUnloadConfirmPanel(const String& message,
                                             Frame* frame);
    
    virtual void runJavaScriptAlert(Frame*, const String&);
    virtual bool runJavaScriptConfirm(Frame*, const String&);
    virtual bool runJavaScriptPrompt(Frame*, const String& message, const String& defaultValue, String& result);
    
    virtual void repaint(const IntRect&, bool contentChanged, bool immediate = false, bool repaintContentOnly = false);
    virtual PlatformPageClient platformPageClient() const;

    virtual void setToolTip(const String&, TextDirection);
private:
    wxWebView* m_webView;
};

}
#endif // ChromeClientWx_H
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
 
#ifndef WXWEBVIEW_H
#define WXWEBVIEW_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "WebFrame.h"
#include "WebKitDefines.h"
#include "WebSettings.h"
#include "ChromeClientWx.h"
#include "FrameLoaderClientWx.h"

class WebViewPrivate;
class WebViewFrameData;
class wxWebFrame;

typedef struct OpaqueJSContext* JSGlobalContextRef;
typedef struct OpaqueJSValue* JSObjectRef;

namespace WebCore {
    class ChromeClientWx;
    class FrameLoaderClientWx;
}

extern WXDLLIMPEXP_WEBKIT const wxChar* wxWebViewNameStr;

static const int defaultCacheCapacity = 8192 * 1024; // mirrors Cache.cpp

class WXDLLIMPEXP_WEBKIT wxWebViewCachePolicy
{
public:
    wxWebViewCachePolicy(unsigned minDead = 0, unsigned maxDead = defaultCacheCapacity, unsigned totalCapacity = defaultCacheCapacity)
        : m_minDeadCapacity(minDead)
        , m_maxDeadCapacity(maxDead)
        , m_capacity(totalCapacity)
    {}

    ~wxWebViewCachePolicy() {}

    unsigned GetCapacity() const { return m_capacity; }
    void SetCapacity(int capacity) { m_capacity = capacity; }

    unsigned GetMinDeadCapacity() const { return m_minDeadCapacity; }
    void SetMinDeadCapacity(unsigned minDeadCapacity) { m_minDeadCapacity = minDeadCapacity; }

    unsigned GetMaxDeadCapacity() const { return m_maxDeadCapacity; }
    void SetMaxDeadCapacity(unsigned maxDeadCapacity) { m_maxDeadCapacity = maxDeadCapacity; }

protected:
    unsigned m_capacity;
    unsigned m_minDeadCapacity;
    unsigned m_maxDeadCapacity;
};


// copied from WebKit/mac/Misc/WebKitErrors[Private].h
enum {
    WebKitErrorCannotShowMIMEType =                             100,
    WebKitErrorCannotShowURL =                                  101,
    WebKitErrorFrameLoadInterruptedByPolicyChange =             102,
    WebKitErrorCannotUseRestrictedPort = 103,
    WebKitErrorCannotFindPlugIn =                               200,
    WebKitErrorCannotLoadPlugIn =                               201,
    WebKitErrorJavaUnavailable =                                202,
};

class WXDLLIMPEXP_WEBKIT wxWebView : public wxWindow
{
    // ChromeClientWx needs to get the Page* stored by the wxWebView
    // for the createWindow function. 
    friend class WebCore::ChromeClientWx;
    friend class WebCore::FrameLoaderClientWx;

public:

    wxWebView(wxWindow* parent, int id = wxID_ANY,
              const wxPoint& point = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style = 0,
              const wxString& name = wxWebViewNameStr); // For wxWebView internal data passing

    wxWebView();
    
    bool Create(wxWindow* parent, int id = wxID_ANY,
                const wxPoint& point = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxWebViewNameStr); // For wxWebView internal data passing
    
    virtual ~wxWebView();
    
    void LoadURL(const wxString& url);
    void Reload();
    
    wxString RunScript(const wxString& javascript);
    
    wxWebFrame* GetMainFrame() { return m_mainFrame; }
    
    bool ShouldClose() const;
      
    static void SetCachePolicy(const wxWebViewCachePolicy& cachePolicy);
    static wxWebViewCachePolicy GetCachePolicy();

    wxWebSettings GetWebSettings();
protected:

    // event handlers (these functions should _not_ be virtual)
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnKeyEvents(wxKeyEvent& event);
    void OnSetFocus(wxFocusEvent& event);
    void OnKillFocus(wxFocusEvent& event);
    
private:
    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxWebView)

    bool m_isInitialized;
    bool m_beingDestroyed;
    WebViewPrivate* m_impl;
    wxWebFrame* m_mainFrame;
    wxString m_title;
    
};

// ----------------------------------------------------------------------------
// Web Kit Events
// ----------------------------------------------------------------------------

enum {
    wxWEBVIEW_LOAD_STARTED = 1,
    wxWEBVIEW_LOAD_NEGOTIATING = 2,
    wxWEBVIEW_LOAD_REDIRECTING = 4,
    wxWEBVIEW_LOAD_TRANSFERRING = 8,
    wxWEBVIEW_LOAD_STOPPED = 16,
    wxWEBVIEW_LOAD_FAILED = 32,
    wxWEBVIEW_LOAD_DL_COMPLETED = 64,
    wxWEBVIEW_LOAD_DOC_COMPLETED = 128,
    wxWEBVIEW_LOAD_ONLOAD_HANDLED = 256,
    wxWEBVIEW_LOAD_WINDOW_OBJECT_CLEARED = 512
};

enum {
    wxWEBVIEW_NAV_LINK_CLICKED = 1,
    wxWEBVIEW_NAV_BACK_NEXT = 2,
    wxWEBVIEW_NAV_FORM_SUBMITTED = 4,
    wxWEBVIEW_NAV_RELOAD = 8,
    wxWEBVIEW_NAV_FORM_RESUBMITTED = 16,
    wxWEBVIEW_NAV_OTHER = 32
};

class WXDLLIMPEXP_WEBKIT wxWebViewBeforeLoadEvent : public wxCommandEvent
{

    DECLARE_DYNAMIC_CLASS( wxWebViewBeforeLoadEvent )


public:
    bool IsCancelled() const { return m_cancelled; }
    void Cancel(bool cancel = true) { m_cancelled = cancel; }
    wxString GetURL() const { return m_url; }
    void SetURL(const wxString& url) { m_url = url; }
    void SetNavigationType(int navType) { m_navType = navType; }
    int GetNavigationType() const { return m_navType; }

    wxWebViewBeforeLoadEvent( wxWindow* win = (wxWindow*) NULL );
    wxEvent *Clone(void) const { return new wxWebViewBeforeLoadEvent(*this); }

private:
    bool m_cancelled;
    wxString m_url;
    int m_navType;
};

class WXDLLIMPEXP_WEBKIT wxWebViewLoadEvent : public wxCommandEvent
{

    DECLARE_DYNAMIC_CLASS( wxWebViewLoadEvent )

public:
    int GetState() const { return m_state; }
    void SetState(const int state) { m_state = state; }
    wxString GetURL() const { return m_url; }
    void SetURL(const wxString& url) { m_url = url; }

    wxWebViewLoadEvent( wxWindow* win = (wxWindow*) NULL );
    wxEvent *Clone(void) const { return new wxWebViewLoadEvent(*this); }

private:
    int m_state;
    wxString m_url;
};

class WXDLLIMPEXP_WEBKIT wxWebViewNewWindowEvent : public wxCommandEvent
{
    DECLARE_DYNAMIC_CLASS( wxWebViewNewWindowEvent )

public:
    wxString GetURL() const { return m_url; }
    void SetURL(const wxString& url) { m_url = url; }
    wxString GetTargetName() const { return m_targetName; }
    void SetTargetName(const wxString& name) { m_targetName = name; }

    wxWebViewNewWindowEvent( wxWindow* win = static_cast<wxWindow*>(NULL));
    wxEvent *Clone(void) const { return new wxWebViewNewWindowEvent(*this); }

private:
    wxString m_url;
    wxString m_targetName;
};

class WXDLLIMPEXP_WEBKIT wxWebViewRightClickEvent : public wxCommandEvent
{
    DECLARE_DYNAMIC_CLASS( wxWebViewRightClickEvent )

public:
    wxWebViewRightClickEvent( wxWindow* win = static_cast<wxWindow*>(NULL));
    wxEvent *Clone(void) const { return new wxWebViewRightClickEvent(*this); }
        
    wxPoint GetPosition() const { return m_position; }
    void SetPosition(wxPoint pos) { m_position = pos; }

private:
    wxPoint m_position;
};

// copied from page/Console.h
enum wxWebViewConsoleMessageLevel {
    TipMessageLevel,
    LogMessageLevel,
    WarningMessageLevel,
    ErrorMessageLevel
};

class WXDLLIMPEXP_WEBKIT wxWebViewConsoleMessageEvent : public wxCommandEvent
{
    DECLARE_DYNAMIC_CLASS( wxWebViewConsoleMessageEvent )

public:
    wxString GetMessage() const { return m_message; }
    void SetMessage(const wxString& message) { m_message = message; }
    
    unsigned int GetLineNumber() const { return m_lineNumber; }
    void SetLineNumber(unsigned int lineNumber) { m_lineNumber = lineNumber; }
    
    wxString GetSourceID() const { return m_sourceID; }
    void SetSourceID(const wxString& sourceID) { m_sourceID = sourceID; }

    wxWebViewConsoleMessageEvent( wxWindow* win = (wxWindow*) NULL );
    wxEvent *Clone(void) const { return new wxWebViewConsoleMessageEvent(*this); }

    wxWebViewConsoleMessageLevel GetLevel() const { return m_level; }
    void SetLevel(wxWebViewConsoleMessageLevel level) { m_level = level; }

private:
    unsigned int m_lineNumber;
    wxString m_message;
    wxString m_sourceID;
    wxWebViewConsoleMessageLevel m_level;
};

class WXDLLIMPEXP_WEBKIT wxWebViewAlertEvent : public wxCommandEvent
{
    DECLARE_DYNAMIC_CLASS( wxWebViewAlertEvent )

public:
    wxString GetMessage() const { return m_message; }
    void SetMessage(const wxString& message) { m_message = message; }

    wxWebViewAlertEvent( wxWindow* win = (wxWindow*) NULL );
    wxEvent *Clone(void) const { return new wxWebViewAlertEvent(*this); }

private:
    wxString m_message;
};

class WXDLLIMPEXP_WEBKIT wxWebViewConfirmEvent : public wxWebViewAlertEvent
{
    DECLARE_DYNAMIC_CLASS( wxWebViewConfirmEvent )

public:   
    int GetReturnCode() const { return m_returnCode; }
    void SetReturnCode(int code) { m_returnCode = code; }

    wxWebViewConfirmEvent( wxWindow* win = (wxWindow*) NULL );
    wxEvent *Clone(void) const { return new wxWebViewConfirmEvent(*this); }

private:
    int m_returnCode;
};

class WXDLLIMPEXP_WEBKIT wxWebViewPromptEvent : public wxWebViewConfirmEvent
{
    DECLARE_DYNAMIC_CLASS( wxWebViewPromptEvent )

public:   
    wxString GetResponse() const { return m_response; }
    void SetResponse(const wxString& response) { m_response = response; }

    wxWebViewPromptEvent( wxWindow* win = (wxWindow*) NULL );
    wxEvent *Clone(void) const { return new wxWebViewPromptEvent(*this); }

private:
    wxString m_response;
};

class WXDLLIMPEXP_WEBKIT wxWebViewReceivedTitleEvent : public wxCommandEvent
{
    DECLARE_DYNAMIC_CLASS( wxWebViewReceivedTitleEvent )

public:
    wxString GetTitle() const { return m_title; }
    void SetTitle(const wxString& title) { m_title = title; }

    wxWebViewReceivedTitleEvent( wxWindow* win = static_cast<wxWindow*>(NULL));
    wxEvent *Clone(void) const { return new wxWebViewReceivedTitleEvent(*this); }

private:
    wxString m_title;
};

class WXDLLIMPEXP_WEBKIT wxWebViewWindowObjectClearedEvent : public wxCommandEvent
{
    DECLARE_DYNAMIC_CLASS( wxWebViewWindowObjectClearedEvent )

public:
    JSGlobalContextRef GetJSContext() const { return m_jsContext; }
    void SetJSContext(JSGlobalContextRef context) { m_jsContext = context; }
    
    JSObjectRef GetWindowObject() const { return m_windowObject; }
    void SetWindowObject(JSObjectRef object) { m_windowObject = object; }

    wxWebViewWindowObjectClearedEvent( wxWindow* win = static_cast<wxWindow*>(NULL));
    wxEvent *Clone(void) const { return new wxWebViewWindowObjectClearedEvent(*this); }

private:
    JSGlobalContextRef m_jsContext;
    JSObjectRef m_windowObject;
};

typedef void (wxEvtHandler::*wxWebViewLoadEventFunction)(wxWebViewLoadEvent&);
typedef void (wxEvtHandler::*wxWebViewBeforeLoadEventFunction)(wxWebViewBeforeLoadEvent&);
typedef void (wxEvtHandler::*wxWebViewNewWindowEventFunction)(wxWebViewNewWindowEvent&);
typedef void (wxEvtHandler::*wxWebViewRightClickEventFunction)(wxWebViewRightClickEvent&);
typedef void (wxEvtHandler::*wxWebViewConsoleMessageEventFunction)(wxWebViewConsoleMessageEvent&);
typedef void (wxEvtHandler::*wxWebViewAlertEventFunction)(wxWebViewAlertEvent&);
typedef void (wxEvtHandler::*wxWebViewConfirmEventFunction)(wxWebViewConfirmEvent&);
typedef void (wxEvtHandler::*wxWebViewPromptEventFunction)(wxWebViewPromptEvent&);
typedef void (wxEvtHandler::*wxWebViewReceivedTitleEventFunction)(wxWebViewReceivedTitleEvent&);
typedef void (wxEvtHandler::*wxWebViewWindowObjectClearedFunction)(wxWebViewWindowObjectClearedEvent&);

#define wxWebViewLoadEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxWebViewLoadEventFunction, &func)
#define wxWebViewBeforeLoadEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxWebViewBeforeLoadEventFunction, &func)
#define wxWebViewNewWindowEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxWebViewNewWindowEventFunction, &func)
#define wxWebViewRightClickEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxWebViewRightClickEventFunction, &func)
#define wxWebViewConsoleMessageEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxWebViewConsoleMessageEventFunction, &func)
#define wxWebViewAlertEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxWebViewAlertEventFunction, &func)
#define wxWebViewConfirmEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxWebViewConfirmEventFunction, &func)
#define wxWebViewPromptEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxWebViewPromptEventFunction, &func)
#define wxWebViewReceivedTitleEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxWebViewReceivedTitleEventFunction, &func)
#define wxWebViewWindowObjectClearedEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxWebViewWindowObjectClearedFunction, &func)

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_WEBKIT, wxEVT_WEBVIEW_BEFORE_LOAD, wxID_ANY)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_WEBKIT, wxEVT_WEBVIEW_LOAD, wxID_ANY)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_WEBKIT, wxEVT_WEBVIEW_NEW_WINDOW, wxID_ANY)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_WEBKIT, wxEVT_WEBVIEW_RIGHT_CLICK, wxID_ANY)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_WEBKIT, wxEVT_WEBVIEW_CONSOLE_MESSAGE, wxID_ANY)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_WEBKIT, wxEVT_WEBVIEW_JS_ALERT, wxID_ANY)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_WEBKIT, wxEVT_WEBVIEW_JS_CONFIRM, wxID_ANY)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_WEBKIT, wxEVT_WEBVIEW_JS_PROMPT, wxID_ANY)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_WEBKIT, wxEVT_WEBVIEW_RECEIVED_TITLE, wxID_ANY)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_WEBKIT, wxEVT_WEBVIEW_WINDOW_OBJECT_CLEARED, wxID_ANY)
END_DECLARE_EVENT_TYPES()

#define EVT_WEBVIEW_LOAD(winid, func)                       \
            DECLARE_EVENT_TABLE_ENTRY( wxEVT_WEBVIEW_LOAD, \
                            winid, \
                            wxID_ANY, \
                            (wxObjectEventFunction)   \
                            (wxWebViewLoadEventFunction) & func, \
                            static_cast<wxObject*>(NULL)),
                            
#define EVT_WEBVIEW_BEFORE_LOAD(winid, func)                       \
            DECLARE_EVENT_TABLE_ENTRY( wxEVT_WEBVIEW_BEFORE_LOAD, \
                            winid, \
                            wxID_ANY, \
                            (wxObjectEventFunction)   \
                            (wxWebViewBeforeLoadEventFunction) & func, \
                            static_cast<wxObject*>(NULL)),
                            
#define EVT_WEBVIEW_NEW_WINDOW(winid, func)                       \
            DECLARE_EVENT_TABLE_ENTRY( wxEVT_WEBVIEW_NEW_WINDOW, \
                            winid, \
                            wxID_ANY, \
                            (wxObjectEventFunction)   \
                            (wxWebViewNewWindowEventFunction) & func, \
                            static_cast<wxObject*>(NULL)),

#define EVT_WEBVIEW_RIGHT_CLICK(winid, func)                       \
            DECLARE_EVENT_TABLE_ENTRY( wxEVT_WEBVIEW_RIGHT_CLICK, \
                            winid, \
                            wxID_ANY, \
                            (wxObjectEventFunction)   \
                            (wxWebViewRightClickEventFunction) & func, \
                            static_cast<wxObject*>(NULL)),
                            
#define EVT_WEBVIEW_CONSOLE_MESSAGE(winid, func)                       \
            DECLARE_EVENT_TABLE_ENTRY( wxEVT_WEBVIEW_CONSOLE_MESSAGE, \
                            winid, \
                            wxID_ANY, \
                            (wxObjectEventFunction)   \
                            (wxWebViewConsoleMessageEventFunction) & func, \
                            static_cast<wxObject*>(NULL)),

#define EVT_WEBVIEW_JS_ALERT(winid, func)                       \
            DECLARE_EVENT_TABLE_ENTRY( wxEVT_WEBVIEW_JS_ALERT, \
                            winid, \
                            wxID_ANY, \
                            (wxObjectEventFunction)   \
                            (wxWebViewAlertEventFunction) & func, \
                            static_cast<wxObject*>(NULL)),
                            
#define EVT_WEBVIEW_JS_CONFIRM(winid, func)                       \
            DECLARE_EVENT_TABLE_ENTRY( wxEVT_WEBVIEW_JS_CONFIRM, \
                            winid, \
                            wxID_ANY, \
                            (wxObjectEventFunction)   \
                            (wxWebViewConfirmEventFunction) & func, \
                            static_cast<wxObject*>(NULL)),
                            
#define EVT_WEBVIEW_JS_PROMPT(winid, func)                       \
            DECLARE_EVENT_TABLE_ENTRY( wxEVT_WEBVIEW_JS_PROMPT, \
                            winid, \
                            wxID_ANY, \
                            (wxObjectEventFunction)   \
                            (wxWebViewPromptEventFunction) & func, \
                            static_cast<wxObject*>(NULL)),
                            
#define EVT_WEBVIEW_RECEIVED_TITLE(winid, func)                       \
            DECLARE_EVENT_TABLE_ENTRY( wxEVT_WEBVIEW_RECEIVED_TITLE, \
                            winid, \
                            wxID_ANY, \
                            (wxObjectEventFunction)   \
                            (wxWebViewReceivedTitleEventFunction) & func, \
                            static_cast<wxObject*>(NULL)),
                            
#define EVT_WEBVIEW_WINDOW_OBJECT_CLEARED(winid, func)                       \
            DECLARE_EVENT_TABLE_ENTRY( wxEVT_WEBVIEW_WINDOW_OBJECT_CLEARED, \
                            winid, \
                            wxID_ANY, \
                            (wxObjectEventFunction)   \
                            (wxWebViewWindowObjectClearedFunction) & func, \
                            static_cast<wxObject*>(NULL)),

#endif // ifndef WXWEBVIEW_H
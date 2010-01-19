#include "wx/wxprec.h"
#include "wx/splitter.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "bowline_webkit.h"

IMPLEMENT_DYNAMIC_CLASS(BowlineWebKit, wxControl)

IMPLEMENT_DYNAMIC_CLASS( wxWebKitScriptEvent, wxCommandEvent )

wxDEFINE_EVENT( wxEVT_WEBKIT_SCRIPT, wxWebKitScriptEvent );

wxWebKitScriptEvent::wxWebKitScriptEvent( wxWindow* win )
{
  SetEventType( wxEVT_WEBKIT_SCRIPT);
  if ( win )
  {
    SetEventObject( win );
    SetId(win->GetId());
  }
}
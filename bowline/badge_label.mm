#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "badge_label.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>
#include <wx/splitter.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#import <Cocoa/Cocoa.h>

#include "badge_label.h"
 
void wxSetBadgeLabel(wxString label) {
#if wxUSE_UNICODE
    NSString *temp = [NSString stringWithUTF8String:label.mb_str(wxConvUTF8)];
#else
    NSString *temp = [NSString stringWithCString:label.c_str() length:label.Len()];
#endif
    NSDockTile *dockTile = [[NSApplication sharedApplication] dockTile];
    [dockTile setBadgeLabel:temp];
}
 
void wxRemoveBadgeLabel() {
    NSDockTile *dockTile = [[NSApplication sharedApplication] dockTile];
    [dockTile setBadgeLabel:nil];
}
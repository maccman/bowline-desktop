#import <Cocoa/Cocoa.h>
 
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
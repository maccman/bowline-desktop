#include "bowline/bowline.h"
#include <wx/utils.h>
#include <wx/socket.h>

using namespace Rice;

Object bowline_host_name(){
  wxString hostName = wxGetFullHostName();
  return hostName;
}

Object bowline_host_ip(){
  wxIPV4address addr;
  addr.Hostname(wxGetFullHostName());
  wxString ipAddr = addr.IPAddress()
  return ipAddr;
}

Object bowline_host_public_ip(){
  wxIPV4address remote;
  remote.Hostname(_("www.google.com"));
  remote.Service(80);

  wxIPV4address local;

  wxSocketClient client;
  if(client.Connect(remote)) client.GetLocal(local);

  wxString ipAddr = local.IPAddress();
  return ipAddr;
}

extern "C" 
void Init_Bowline_Host(){
  Class rb_cBowlineHost =
    define_class("Bowline::Host")
    .define_singleton_method("name", &bowline_host_name)
    .define_singleton_method("ip", &bowline_host_ip)
    .define_singleton_method("public_ip", &bowline_host_public_ip)
}
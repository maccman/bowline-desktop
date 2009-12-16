#include "bowline.h"
#include <wx/utils.h>
#include <wx/socket.h>

using namespace Rice;

wxString bowline_host_name(){
  wxString hostName = wxGetFullHostName();
  return hostName;
}

wxString bowline_host_ip(){
  wxIPV4address addr;
  addr.Hostname(wxGetFullHostName());
  wxString ipAddr = addr.IPAddress();
  return ipAddr;
}

wxString bowline_host_public_ip(){
  wxIPV4address remote;
  remote.Hostname(_("www.google.com"));
  remote.Service(80);

  wxIPV4address local;

  wxSocketClient client;
  if(client.Connect(remote)) client.GetLocal(local);

  wxString ipAddr = local.IPAddress();
  return ipAddr;
}

void Init_Bowline_Host(){
  Module rb_mBowline        = define_module("Bowline");
  Module rb_mBowlineDesktop = define_module_under(rb_mBowline, "Desktop");
  
  Class rb_cBowlineHost =
    define_class_under(rb_mBowlineDesktop, "Host")
    .define_singleton_method("name",      &bowline_host_name)
    .define_singleton_method("ip",        &bowline_host_ip)
    .define_singleton_method("public_ip", &bowline_host_public_ip);
}
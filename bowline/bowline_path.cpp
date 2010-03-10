#include "bowline.h"
#include <wx/stdpaths.h>

using namespace Rice;

wxString bowline_path_exe(){
  return wxStandardPaths::Get().GetExecutablePath();
}

wxString bowline_path_documents(){
  return wxStandardPaths::Get().GetDocumentsDir();
}

wxString bowline_path_data(){
  return wxStandardPaths::Get().GetDataDir();
}

wxString bowline_path_user_data(){
  return wxStandardPaths::Get().GetUserDataDir();
}

wxString bowline_path_temp(){
  return wxStandardPaths::Get().GetTempDir();
}

void Init_Bowline_Path(){
  Module rb_mBowline        = define_module("Bowline");
  Module rb_mBowlineDesktop = define_module_under(rb_mBowline, "Desktop");
  
  Module rb_mBowlineApp =
    define_module_under(rb_mBowlineDesktop, "Path")
    .define_module_function("exe",        &bowline_path_exe)
    .define_module_function("documents",  &bowline_path_documents)
    .define_module_function("data",       &bowline_path_data)
    .define_module_function("user_data",  &bowline_path_user_data)
    .define_module_function("temp",       &bowline_path_temp);
}
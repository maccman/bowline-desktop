#ifndef BOWLINE_CONFIG_CPP_8U7DUT7X
#define BOWLINE_CONFIG_CPP_8U7DUT7X

class BowlineConfig
{
public:
  static VALUE get(const wxString& name){
    int error;
    wxString rubyCode = _("Bowline.configuration.");
    rubyCode.append(name);
    VALUE result = rb_eval_string_protect(rubyCode.c_str(), &error);
    if(error) {
      RubyUtils::LogError();
      throw "Config Error";
    }
    return result;
  }

  static int getInt(const wxString& name){
    return NUM2INT(BowlineConfig::get(name));
  }
  
  static wxString getString(const wxString& name){
    VALUE rString = BowlineConfig::get(name);
    wxString cString = StringValueCStr(rString);
    return cString;
  }
  
  static bool getBool(const wxString& name){
    VALUE rBool = BowlineConfig::get(name);
    if(rBool == Qtrue) return true;
    return false;
  }
};

#endif /* end of include guard: BOWLINE_CONFIG_CPP_8U7DUT7X */
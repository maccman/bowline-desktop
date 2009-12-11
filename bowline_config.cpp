class BowlineConfig
{
public:
  static VALUE get(const wxString& name){
    int error;
    VALUE result = rb_eval_string_protect(
      "Bowline.configuration." + name, 
      &error
    );
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
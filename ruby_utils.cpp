#include <iostream>

class RubyUtils
{
public:
  static void LogError(){
    VALUE lasterr = rb_gv_get("$!");
    VALUE message = rb_obj_as_string(lasterr);
    std::cout << RSTRING_PTR(message) << std::endl;
  }
};
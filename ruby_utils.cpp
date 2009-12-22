#include <iostream>

class RubyUtils
{
public:
  static void LogError(){
    VALUE lasterr = rb_gv_get("$!");
    VALUE message = rb_obj_as_string(lasterr);
    VALUE array   = rb_funcall(lasterr, rb_intern("backtrace"), 0);
    VALUE backtrace = rb_funcall(array, rb_intern("join"), 1, rb_str_new2("\n\t"));
    std::cout << "\t" << RSTRING_PTR(message) << std::endl;
    std::cout << RSTRING_PTR(backtrace) << std::endl;
  }
};
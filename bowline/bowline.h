#ifndef BOWLINE_H_TVWBTAN8
#define BOWLINE_H_TVWBTAN8

#include <rice/Object.hpp>
#include <rice/Module.hpp>
#include <rice/Constructor.hpp>
#include <rice/Class.hpp>
#include <rice/Array.hpp>
#include <rice/String.hpp>

template<> 
Rice::Object to_ruby(wxString const& str) {
  return to_ruby<const char*>(str.c_str());
}

template<>
wxString from_ruby<wxString>(Rice::Object x) {
  return wxString(from_ruby<const char*>(x));
}

template<>
wxSize from_ruby<wxSize>(Rice::Object x) {
  Rice::Array arr = from_ruby<Rice::Array>(x);
  return wxSize(from_ruby<int>(arr[0]), from_ruby<int>(arr[1]));
}

// TODO - wxSize

#endif /* end of include guard: BOWLINE_H_TVWBTAN8 */
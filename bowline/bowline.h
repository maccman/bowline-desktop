#ifndef BOWLINE_H_TVWBTAN8
#define BOWLINE_H_TVWBTAN8

#include "rice/Object.hpp"
#include "rice/Class.hpp"
#include "rice/String.hpp"

template<> 
Rice::Object to_ruby(wxString const& str) {
  return to_ruby<const char*>(str.c_str());
}

template<>
wxString from_ruby<wxString>(Rice::Object x) {
  // return wxString(x);
  return wxEmptyString;
}

#endif /* end of include guard: BOWLINE_H_TVWBTAN8 */
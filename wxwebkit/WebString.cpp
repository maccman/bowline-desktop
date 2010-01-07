#include "WebString.h"
#include "CString.h"
#include "unicode/ustring.h"

WebString::WebString(WebCore::String str){
  string = wxString(str.utf8().data(), wxConvUTF8);
}
  
WebString::operator WebCore::String() {
  #if !wxUSE_UNICODE
      #error "This code only works in Unicode build of wxWidgets"
  #endif

  // ICU's UChar is 16bit wide, UTF-16, and the code below counts on it, so
  // it would break if the definition changed:
  wxCOMPILE_TIME_ASSERT(sizeof(UChar) == 2, UCharSizeMustBe16Bit);

  #if SIZEOF_WCHAR_T == 2 // wchar_t==UChar

    const UChar* str = wxstr.wc_str();
    const size_t len = wxstr.length();

  #else // SIZEOF_WCHAR_T == 4

    // NB: we can't simply use wxstr.mb_str(wxMBConvUTF16()) here because
    //     the number of characters in UTF-16 encoding of the string may differ
    //     from the number of UTF-32 values and we can't get the length from
    //     returned buffer:

    #if defined(wxUSE_UNICODE_UTF8) && wxUSE_UNICODE_UTF8
        // in wx3's UTF8 mode, wc_str() returns a buffer, not raw pointer
      wxWCharBuffer widestr(string.wc_str());
    #else
      const wxChar *widestr = string.wc_str();
    #endif
    const size_t widelen = string.length();

    // allocate buffer for the UTF-16 string:
    wxMBConvUTF16 conv;
    const size_t utf16bufLen = conv.FromWChar(NULL, 0, widestr, widelen);
    wxCharBuffer utf16buf(utf16bufLen);

    // and convert wxString to UTF-16 (=UChar*):
    const UChar* str = (const UChar*)utf16buf.data();
    size_t len = conv.FromWChar(utf16buf.data(), utf16bufLen, widestr, widelen) / 2;

  #endif // SIZEOF_WCHAR_T == 4

  // conversion to UTF-16 or getting internal buffer isn't supposed to fail:
  wxASSERT_MSG(str != NULL, _T("failed string conversion?"));

  return WebCore::StringImpl::create(str, len);
}
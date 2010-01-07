#ifndef WEBRECT_H_WU2NYAR3
#define WEBRECT_H_WU2NYAR3

#include "config.h"
#include "FloatRect.h"
#include "IntRect.h"

#include <wx/defs.h>
#include <wx/gdicmn.h>

class WebRect
{
public:
  WebRect (WebCore::IntRect r) {
    rect = wxRect(r.x(), r.y(), r.width(), r.height());
  }
  WebRect (wxRect r) { rect = r; }

  operator WebCore::IntRect() {
    return WebCore::IntRect(
      WebCore::FloatRect(rect.x, rect.y, rect.width, rect.height)
    );
  }
  operator wxRect(){ return rect; }
private:
  wxRect rect;
};

#endif /* end of include guard: WEBRECT_H_WU2NYAR3 */
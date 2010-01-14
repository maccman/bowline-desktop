#include "config.h"
#include "EmptyClients.h"
#include "HTMLFrameOwnerElement.h"
#include "Page.h"
#include "Frame.h"
#include "Logging.h"
#include "ResourceRequest.h"
#include "Chrome.h"

namespace WebCore {

class FrameLoaderClientWx : public EmptyFrameLoaderClient
{
public:
  FrameLoaderClientWx (){ };
  virtual ~FrameLoaderClientWx (){ };
  
  bool hasWebView() const
  {
      return true;
  }
  
  void setFrame(RefPtr<WebCore::Frame> &frame)
  {
      m_frame = frame;
  }
  
  void transitionToCommittedForNewPage()
  { 
      ASSERT(m_frame);

      IntSize size = IntRect(FloatRect(0, 0, 100, 100)).size();
      bool transparent = false;
      Color backgroundColor = transparent ? WebCore::Color::transparent : WebCore::Color::white;

      if (m_frame)
          m_frame->createView(size, backgroundColor, transparent, IntSize(), false); 
  }

private:
  RefPtr<WebCore::Frame> m_frame;
};

}

int main (int argc, char const *argv[])
{
  
  WTF::initializeThreading();
  // WebCore::InitializeLoggingChannelsIfNecessary();
  
  WebCore::Page *page = new WebCore::Page(
    new WebCore::EmptyChromeClient(), 
    new WebCore::EmptyContextMenuClient(), 
    new WebCore::EmptyEditorClient(), 
    new WebCore::EmptyDragClient(), 
    new WebCore::EmptyInspectorClient(), 
    0, 0
  );
  
  WebCore::HTMLFrameOwnerElement* parentFrame = 0;
  
  WebCore::FrameLoaderClientWx* loaderClient = new WebCore::FrameLoaderClientWx();
  RefPtr<WebCore::Frame> m_frame = WebCore::Frame::create(page, parentFrame, loaderClient);
  
  loaderClient->setFrame(m_frame);
    
  m_frame->init();
  
  OwnPtr<WebCore::Chrome> m_chrome;
  
  m_chrome = new WebCore::Chrome(page, new WebCore::EmptyChromeClient());
  // m_chrome.get()->scrollbarsModeDidChange();
  
  const WebCore::Chrome *chrome2 = new WebCore::Chrome(page, new WebCore::EmptyChromeClient());
  chrome2->scrollbarsModeDidChange();
  
  page->chrome()->scrollbarsModeDidChange();
  
  return 0;
}
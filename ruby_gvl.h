#ifndef _RUBY_GVL_H_
#define _RUBY_GVL_H_

extern "C" {
  struct rb_blocking_region_buffer *rb_thread_blocking_region_begin(void);
  void rb_thread_blocking_region_end(struct rb_blocking_region_buffer *region); 
}

class RubyGVL
{ 
public:
  RubyGVL();
  static bool Lock();
  static bool Unlock();
  static void Release();
  virtual ~RubyGVL();
  static bool locked;
private:
  bool did_lock;
  static struct rb_blocking_region_buffer *curb_lock;
  static wxMutex mutex;
  static bool released;
};

#endif
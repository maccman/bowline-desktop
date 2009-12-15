#include "ruby_gvl.h"

struct rb_blocking_region_buffer *RubyGVL::curb_lock = NULL;
bool RubyGVL::locked = true;
bool RubyGVL::released = false;
wxMutex RubyGVL::mutex;

RubyGVL::RubyGVL(){
  if(!RubyGVL::released) return;
  // RubyGVL::mutex.Lock();
  did_lock = RubyGVL::Lock();
}

RubyGVL::~RubyGVL(){
  if(!did_lock) return;
  RubyGVL::Unlock();
  // RubyGVL::mutex.Unlock();
}

bool RubyGVL::Lock(){
  if(locked) {
    // std::cout << "Already locked\n";
    return false;
  }
  // std::cout << "Locking\n";
  locked = true;
  rb_thread_blocking_region_end(curb_lock);
  return true;
}

bool RubyGVL::Unlock(){
  if(!locked) {
    // std::cout << "Already unlocked\n";
    return false;
  }
  // std::cout << "Unlocking\n";
  locked = false;
  curb_lock = rb_thread_blocking_region_begin();
  return true;
}

void RubyGVL::Release(){
  released = true;
}
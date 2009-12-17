
struct rb_blocking_region_buffer *RubyGVL::curb_lock = NULL;
bool RubyGVL::locked = true;

RubyGVL::RubyGVL(){
  RubyGVL::Lock();
}

RubyGVL::~RubyGVL(){
  RubyGVL::Unlock();
}

void RubyGVL::Lock(){
  if(locked) {
    return;
  }
  locked = true;
  rb_thread_blocking_region_end(curb_lock);
}

void RubyGVL::Unlock(){
  if(!locked) {
    return;
  }
  locked = false;
  curb_lock = rb_thread_blocking_region_begin();
}
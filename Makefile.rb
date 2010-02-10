raise "Only supports Ruby 1.9.1" if RUBY_VERSION !~ /^1\.9/

require 'rbconfig'
# gem 'rice', '>= 1.3.0'

# rice_gem = Gem.cache.find_name('rice').first

includes = []
includes << RbConfig::CONFIG['rubyhdrdir']
includes << File.join(RbConfig::CONFIG['rubyhdrdir'], RUBY_PLATFORM)
includes << "."

opts = includes.map {|inc| "-I#{inc}" }
opts << `wx-config --cxxflags`.chomp
opts << "-Flibs"

libs = []
libs << `wx-config --libs`.chomp

libs << "-Flibs"
libs << RbConfig::CONFIG['LIBRUBYARG_STATIC']
libs << "-L."
libs << "-lrice"
libs << "-framework JavaScriptCore"
libs << "-framework WebKit"

DEBUG_FLAGS      = "-g -Wall -Wcast-align -Wmissing-noreturn -Wundef -Wshorten-64-to-32"
STANDARD_FLAGS   = "-arch i386 -fmessage-length=0 -Wno-trigraphs -fpascal-strings -mmacosx-version-min=10.5"
PRODUCTION_FLAGS = "-DNDEBUG"

vars :CC => "g++", :FLAGS => [DEBUG_FLAGS, STANDARD_FLAGS], :LIBS => libs, :OPTS => opts

rule :all, :depends => ["badge_label.o", "bowline_webkit.o", "main.o"] do
  compile :LIBS, :output => "bowline-desktop"
  echo "Done compiling, run ./bowline-desktop"
end

rule "main.o", :depends => ["main.cpp"] do
  compile :to_obj, :OPTS, :$@
end

rule "badge_label.o", :depends => ["bowline/badge_label.h", "bowline/badge_label.mm"] do
  compile :to_obj, :OPTS, :$@
end

rule "bowline_webkit.o", :depends => [
  "bowline/bowline_webkit.h", 
  "bowline/bowline_webkit_mac.mm"
] do
  compile :to_obj, :OPTS, :$@
end

rule :test, :depends => ["test.cpp"] do
  compile :LIBS, :OPTS, :$@
end

clean "*.o", "bowline-desktop", "test"

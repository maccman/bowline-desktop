raise "Only supports Ruby 1.9.1" if RUBY_VERSION != "1.9.1"

require 'rbconfig'
gem 'rice', '>= 1.3.0'

rice_gem = Gem.cache.find_name('rice').first

opts  = `wx-config --cxxflags`.chomp
opts  += " -I#{RbConfig::CONFIG['rubyhdrdir']}"
opts  += " -I" + File.join(RbConfig::CONFIG['rubyhdrdir'], RUBY_PLATFORM)
opts  += " -I" + File.join(rice_gem.full_gem_path, *%w{ruby lib include})

libs = `wx-config --libs`.chomp
libs += " " + RbConfig::CONFIG['LIBRUBYARG_STATIC']
libs += " -L" + File.join(rice_gem.full_gem_path, *%w{ruby lib lib})
libs += " -lrice"

vars :CC => "g++", :FLAGS => "-g", :LIBS => libs, :OPTS => opts

rule :all, :depends => ["badge_label.o", "webkit.o", "main.o"] do
  compile :LIBS, :output => "bowline-desktop"
  echo "Done compiling, run ./bowline-desktop"
end

rule "main.o", :depends => ["main.cpp"] do
  compile :to_obj, :OPTS
end
rule "badge_label.o", :depends => ["bowline/badge_label.h", "bowline/badge_label.mm"] do
  compile :to_obj, :OPTS
end
rule "webkit.o", :depends => ["bowline/webkit/webkit.h", "bowline/webkit/webkit.mm"] do
  compile :to_obj, :OPTS
end

clean "*.o", "bowline-desktop"

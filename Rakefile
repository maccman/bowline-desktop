require 'rbconfig'
gem 'rice', '>= 1.3.0'

rice_gem = Gem.cache.find_name('rice').first

task :default => :build
desc "Build Bowline"
task :build do
  opts  = `wx-config --libs`.chomp
  opts  += `wx-config --cxxflags`.chomp
  opts  += " -I#{RbConfig::CONFIG['rubyhdrdir']}"
  opts  += " -I" + File.join(RbConfig::CONFIG['rubyhdrdir'], RUBY_PLATFORM)
  opts  += " -lruby19-static"
  opts  += " -I" + File.join(rice_gem.full_gem_path, *%w{ruby lib include})
  opts  += " -L" + File.join(rice_gem.full_gem_path, *%w{ruby lib lib})
  opts  += " -lrice"
  puts "g++ main.cpp #{opts} -o bowline-desktop"
  `g++ main.cpp #{opts} -o bowline-desktop`
end

desc "Run Bowline"
task :run do
  exec './main'
end
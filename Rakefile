task :default => :build
desc "Build Bowline"
task :build do
  # TODO - don't hardcode paths
  opts  = `wx-config --libs`.chomp
  opts  += `wx-config --cxxflags`.chomp
  opts  += ' -I/usr/local/include/ruby19-1.9.1'
  opts  += ' -I/usr/local/include/ruby19-1.9.1/i386-darwin9.7.0'
  opts  += ' -lruby19-static'
  opts  += ' -I/usr/local/lib/ruby19/gems/1.9.1/gems/rice-1.3.0/ruby/lib/include'
  opts  += ' -L/usr/local/lib/ruby19/gems/1.9.1/gems/rice-1.3.0/ruby/lib/lib'
  opts  += ' -lrice'
  `g++ main.cpp #{opts} -o bowline-desktop`
end

desc "Run Bowline"
task :run do
  exec './main'
end
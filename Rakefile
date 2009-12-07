task :default => :build
desc "Build Bowline"
task :build do
  opts  = `wx-config --libs`.chomp
  opts  += `wx-config --cxxflags`.chomp
  opts  += ' -I/usr/local/lib/ruby19/1.9.1/i386-darwin9.7.0'
  opts  += ' -I/usr/local/include/ruby19-1.9.1'
  opts  += ' -I/usr/local/include/ruby19-1.9.1/i386-darwin9.7.0'
  opts  += ' -lruby19'
  `g++ main.cpp #{opts} -o main`
end

desc "Run Bowline"
task :run do
  exec './main'
end
raise "Only supports Ruby 1.9.1" if RUBY_VERSION !~ /^1\.9/

require 'rbconfig'
gem 'rice', '>= 1.3.0'

rice_gem = Gem.cache.find_name('rice').first

includes = []
includes << RbConfig::CONFIG['rubyhdrdir']
includes << File.join(RbConfig::CONFIG['rubyhdrdir'], RUBY_PLATFORM)
includes << File.join(rice_gem.full_gem_path, *%w{ruby lib include})
includes << "wxwebkit"

opts = includes.map {|inc| "-I#{inc}" }
opts << `wx-config --cxxflags`.chomp

libs = [`wx-config --libs`.chomp]
libs << RbConfig::CONFIG['LIBRUBYARG_STATIC']
libs << "-L" + File.join(rice_gem.full_gem_path, *%w{ruby lib lib})
libs << "-lrice"
libs << "-L."
libs << "-lwxwebkit"

# TODO - make configurable
webkit_path = "/Users/Alex/Downloads/WebKit-r52751/"

wlibs = [`wx-config --libs`.chomp]
# wlibs += " -framework WebKit"
# wlibs += " -framework JavaScriptCore"
wlibs << "-L."
wlibs << "-L" + File.join(webkit_path, "WebKitLibraries")
wlibs << "-L" + File.join(webkit_path, "WebKitLibraries", "unix", "lib")
wlibs << "-L/opt/local/lib"
wlibs << "-licucore"
wlibs << "-lxml2"
wlibs << "-lcurl"
wlibs << "-lsqlite3"
wlibs << "-lpng"
wlibs << "-lxslt"
wlibs << "-F."
wlibs << "-framework JavaScriptCore"
wlibs << "-framework WebCore"

wincludes = %w{
  JavaScriptCore/wtf/unicode/icu
  JavaScriptCore/wtf/unicode
  JavaScriptCore/wtf
  JavaScriptCore/runtime
  JavaScriptCore/profiler
  JavaScriptCore/pcre
  JavaScriptCore/wtf/unicode/icu
  JavaScriptCore/parser
  JavaScriptCore/wtf/unicode
  JavaScriptCore/jit
  JavaScriptCore/wtf
  JavaScriptCore/interpreter
  JavaScriptCore/runtime
  JavaScriptCore/DerivedSources
  JavaScriptCore/profiler
  JavaScriptCore/pcre
  JavaScriptCore/parser
  JavaScriptCore/jit
  JavaScriptCore/interpreter
  JavaScriptCore/DerivedSources
  JavaScriptCore/debugger
  JavaScriptCore/bytecompiler
  JavaScriptCore/bytecode
  JavaScriptCore/API
  JavaScriptCore/assembler
  WebCore/DerivedSources
  WebCore/xml
  WebCore/websockets
  WebCore/storage
  WebCore/rendering/style
  WebCore/rendering
  WebCore/plugins
  WebCore/platform/text
  WebCore/platform/sql
  WebCore/platform/network
  WebCore/platform/mock
  WebCore/platform/image-decoders/zlib
  WebCore/platform/image-decoders/xbm
  WebCore/platform/image-decoders/png
  WebCore/platform/image-decoders/jpeg
  WebCore/platform/image-decoders/ico
  WebCore/platform/image-decoders/gif
  WebCore/platform/image-decoders/bmp
  WebCore/platform/image-decoders
  WebCore/platform/graphics/transforms
  WebCore/platform/graphics
  WebCore/platform/animation
  WebCore/platform
  WebCore/page/animation
  WebCore/page
  WebCore/notifications
  WebCore/loader/icon
  WebCore/loader/archive
  WebCore/loader/appcache
  WebCore/loader
  WebCore/inspector
  WebCore/html/canvas
  WebCore/html
  WebCore/history
  WebCore/editing
  WebCore/dom/default
  WebCore/dom
  WebCore/css
  WebCore/bridge/c
  WebCore/bridge
  WebCore/bindings/js
  WebCore/bindings
  WebCore/accessibility
  WebCore/platform/graphics/mac
  WebCore/platform/bridge/mac
  WebCore/platform/mac
  WebCore/platform/network/curl
  WebCore/page/mac
  WebCore
  JavaScriptCore
  WebKitLibraries
  WebKitLibraries/unix/include
  WebKitLibraries/WebCoreSQLite3
  JavaScriptCore/icu
  WebCore/icu
  WebCore/platform/cf
}

# WebKitBuild/Debug ?

wincludes = wincludes.map {|inc| File.join(webkit_path, inc) }

wincludes << File.join(webkit_path, *%w{ WebKitBuild Release DerivedSources WebCore })

wincludes << "wxwebkit"
wincludes << "wxwebkit/WebKitSupport"

wincludes << "/opt/local/include" # ?
wincludes << "/opt/local/include/libxml2" # ?

wopts = wincludes.map {|inc| "-I#{inc}" }

wopts << "-D__DYNAMIC__" 
wopts << "-D__DYNAMIC__" 
wopts << "-DU_DISABLE_RENAMING=1" 
wopts << "-DPIC"
wopts << "-DWXMAKINGDLL_WEBKIT" 
wopts << "-DWTF_USE_CURL" 
wopts << "-DWTF_USE_PTHREADS" 
wopts << "-DWTF_USE_WXGC" 
wopts << "-DENABLE_DATABASE" 
wopts << "-DENABLE_XSLT" 
wopts << "-DENABLE_JAVASCRIPT_DEBUGGER" 
wopts << "-D_FILE_OFFSET_BITS=64"
wopts << "-DWXUSINGDLL" 
wopts << "-D__WXOSX_CARBON__"
wopts << "-D_FILE_OFFSET_BITS=64"
wopts << "-DWXUSINGDLL"
wopts << "-D__WXOSX_CARBON__"
wopts << "-D__private_extern__=extern"

# We don't want icu to rename itself, 
# otherwise it won't work with OSX's native copy 
wopts << "-DU_DISABLE_RENAMING"

wopts << `wx-config --cxxflags`.chomp

wopts << "-F/System/Library/Frameworks/ApplicationServices.framework/Frameworks"

# TODO - remove
# -isysroot /Developer/SDKs/MacOSX10.5.sdk 
# -fPIC 
# -feliminate-unused-debug-symbols  
# # -dumpbase WebView.cpp -dumpbase 
# -mmacosx-version-min=10.5 
# -mtune=generic 
# -march=apple 
# -auxbase-strip 
# #-auxbase WebView 
# -fPIC -feliminate-unused-debug-symbols -fPIC -fPIC

wfiles  = Dir["wxwebkit/*.cpp"]
wfiles  << "wxwebkit/WebKitSupport/FrameLoaderClientWx.cpp"
wofiles = wfiles.map {|path| path.gsub(/\.cpp$/, ".o") }

vars :CC => "g++", :FLAGS => "-g -Wall", :LIBS => libs, :OPTS => opts, :WOPTS => wopts, :WLIBS => wlibs

rule :all, :depends => ["badge_label.o", "main.o"] do
  compile :LIBS, :output => "bowline-desktop"
  echo "Done compiling, run ./bowline-desktop"
end

rule "main.o", :depends => ["main.cpp"] do
  compile :to_obj, :OPTS
end
rule "badge_label.o", :depends => ["bowline/badge_label.h", "bowline/badge_label.mm"] do
  compile :to_obj, :OPTS
end

wfiles.zip(wofiles).each do |(path, opath)|
  rule(opath, :depends => path) do
    compile :to_obj, :WOPTS, :$@
  end
end

rule :wxwebkit, :depends => wofiles do
  compile :WLIBS, "-dynamiclib", :output => "libwxwebkit.dylib"
end

# TODO - remove
# rule :test, :depends => "test.o" do
#   compile :LIBS, :output => "test"
# end
# 
# rule "test.o", :depends => "wxwebkit/test.cpp" do
#   compile :to_obj, :WOPTS, :$@
# end

clean "*.o", "wxwebkit/*.o", "wxwebkit/WebKitSupport/*.o", "libwxwebkit.dylib", "bowline-desktop"

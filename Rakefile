# Change paths in WebKit libraries

namespace :libs do
  task :id do
    libs = %w{ JavaScriptCore JavaScriptGlue WebCore WebKit }
    libs.each do |lib|
      `install_name_tool -id \
        @rpath/#{lib}.framework/Versions/A/#{lib} \
        libs/#{lib}.framework/Versions/A/#{lib}`
    end
  end

  task :change do
    webkit_dir = ENV["WEBKIT_PATH"] || "/Volumes/Data/WebKit/53765/10.6"
    
    libs = %w{ JavaScriptCore JavaScriptGlue WebCore WebKit }
    libs.each do |lib|
      
      puts "Changing #{webkit_dir}/#{lib}.framework"
      
      (libs - [lib]).each do |other|
        `install_name_tool -change \
          #{webkit_dir}/#{other}.framework/Versions/A/#{other} \
          @rpath/#{other}.framework/Versions/A/#{other} \
          libs/#{lib}.framework/Versions/A/#{lib}`
      end
    end
  end
end

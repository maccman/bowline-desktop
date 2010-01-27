# Change paths in WebKit libraries

namespace :libs do
  task :id do
    libs = %w{ JavaScriptCore JavaScriptGlue WebCore WebKit }
    libs.each do |lib|
      `install_name_tool -id \
        @executable_path/libs/#{lib}.framework/Versions/A/#{lib} \
        libs/#{lib}.framework/Versions/A/#{lib}`
    end
  end

  task :change do
    libs = %w{ JavaScriptCore JavaScriptGlue WebCore WebKit }
    libs.each do |lib|
      (libs - [lib]).each do |other|
        `install_name_tool -change \
          /Volumes/Data/WebKit/53765/10.6/#{other}.framework/Versions/A/#{other} \
          @executable_path/libs/#{other}.framework/Versions/A/#{other} \
          libs/#{lib}.framework/Versions/A/#{lib}`
      end
    end
  end
end
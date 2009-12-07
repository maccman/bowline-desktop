module Bowline
  module Desktop
    module JS
      module ClassMethods
        def expose_js(opts = {})
          # Define method "js_call?"
        end
      end
    
      class Script
        attr_reader :str, :prok
        def initialize(str, prok = nil)
          @str, @prok = str, prok
        end
      
        def call
          result = run_js_script(str)
          Thread.new { prok.call(result) } if prok
          result
        end
      end
    
      # Poll JS variable
      # Deserialize JSON
      # Find class
      # Check if RPC is supported
      # Call method (class)
      # Call js method with result
    
      def poll
        run_scripts
      end
      module_function :poll
      Desktop.on_idle(method(:poll))
    
      def eval(str, method = nil, &block)
        script = Script.new(str, method||block)
        if Thread.current == Thread.main
          script.call
        else
          scripts << script
        end
      end
      module_function :eval
    
      private
        def run_scripts
          while script = scripts.shift
            script.call
          end
        end
        module_function :run_scripts
      
        def scripts
          Thread.main[:scripts] ||= []
        end
        module_function :scripts
    end
  end
end
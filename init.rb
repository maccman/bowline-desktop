puts "Loaded init.rb"

Thread.abort_on_exception = true

module Bowline
  Thread.main[:scripts] = []
  
  def execute(str)
    if Thread.current == Thread.main
      run_script(str)
    else
      Thread.main[:scripts] << str
    end
  end
  module_function :execute
  
  def idle
    while str = Thread.main[:scripts].shift
      run_script(str)
    end
  end
  module_function :idle
  
  def loaded
    puts "On load!"
  end
  module_function :loaded
end

Thread.new do
  10.times do |n|
    puts n
    if n == 6
      Bowline.execute("document.body.style.background = 'red'")
    end
    sleep 1
  end
end
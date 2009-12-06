puts "Loaded init.rb"

Thread.new do
  10.times do |n|
    puts n
    sleep 1
  end
end
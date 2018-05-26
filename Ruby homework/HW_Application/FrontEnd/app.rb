require_relative "../require/network/front.rb"

client = Client_QA.new("network_config.cfg", true)

handler = Proc.new do |a, i|
    print "> Q: "
    str = gets.chomp
    {:response => str}
end

client.connect("hello", handler)

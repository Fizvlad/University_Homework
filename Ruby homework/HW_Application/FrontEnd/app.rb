require_relative "../require/network/front.rb"

client = Client_QA.new("network_config.cfg", true)

handler = Proc.new do |a, i|
    puts a["response"]
    puts a["do_close"]
    puts a["do_stop"]
    puts i

    str = gets.chomp
    return {:response => str}
end

client.connect("hello", handler)

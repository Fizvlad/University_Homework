require_relative "../require/network/back.rb"

server = Server_QA.new("network_config.cfg", true)

handler = Proc.new do |q, i|
    puts q["response"]
    puts i

    str = gets.chomp
    return {:response => str, :do_stop => false, :do_close => str == "exit"}
end

server.listen(handler)

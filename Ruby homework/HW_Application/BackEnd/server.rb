require_relative "../require/network/back.rb"

server = Server_QA.new("network_config.cfg", true)

handler = Proc.new do |q, i|
    print "> A: "
    str = gets.chomp
    {:response => str, :do_stop => str == "stop", :do_close => str == "close"}
end

server.listen(handler)

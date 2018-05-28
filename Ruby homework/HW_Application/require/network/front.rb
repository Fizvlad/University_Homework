#!/usr/bin/ruby -w

# Front-end part of application

require_relative "back_front__shared.rb"


#===============================================================================
# Client
#===============================================================================

class Client_QA
    attr_reader :config
    attr_accessor :do_log

    include Back_Front__shared

    def connect(handler)
        server = TCPSocket.open(@config["hostname"], @config["port"])
        puts "Connection established" if @do_log
        i = 0
        rsa_this = OpenSSL::PKey::RSA.new(2048)
        send(server, {:response => rsa_this.public_key})
        rsa_other = OpenSSL::PKey::RSA.new(receive(server)["response"])


        loop do
            q = handler.call({:response => "", :do_stop => false, :do_close => false}, i)
            # q = {"response": str}
            puts "Q: #{q}" if @do_log
            send(server, q, rsa_other)
            i += 1

            a = receive(server, rsa_this)
            # a = {"response": str, "do_stop": bool, "do_close": bool}
            puts "A: #{a}" if @do_log

            break if a["do_stop"] || a["do_close"]
        end
        server.close
    end
end

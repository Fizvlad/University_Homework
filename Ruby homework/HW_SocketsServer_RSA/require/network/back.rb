#!/usr/bin/ruby -w

# Back-end part of application

require_relative "back_front__shared.rb"


#===============================================================================
# Server
#===============================================================================

class Server_QA
    attr_reader :config
    attr_accessor :do_log

    include Back_Front__shared

    def listen(handler)
        stop_flag = false
        server = TCPServer.open(@config["port"])
        puts "Server is running" if @do_log
        while !stop_flag do
            newClient = server.accept
            Thread.start(newClient) do |client|
                puts "New client connected" if @do_log
                close_flag = false
                i = 0
                rsa_this = OpenSSL::PKey::RSA.new(2048)
                rsa_other = OpenSSL::PKey::RSA.new(receive(client)["response"])
                send(client, {:response => rsa_this.public_key})


                loop do
                    break if close_flag || stop_flag

                    q = receive(client, rsa_this)
                    # q = {"response": str}
                    puts "Q: #{q}" if @do_log
                    a = handler.call(q, i)
                    # a = {"response": str, "do_stop": bool, "do_close": bool}
                    puts "A: #{a}" if @do_log
                    i += 1

                    if a[:do_stop]
                        stop_flag = true
                    elsif a[:do_close]
                        close_flag = true
                    end
                    send(client, a, rsa_other)
                end
                # Need to initiate self connection to stop server.accept from listening
                if stop_flag
                    puts "Server is stopping"
                    TCPSocket.open(@config["hostname"], @config["port"]).close
                end
            end
        end
        server.close
    end
end

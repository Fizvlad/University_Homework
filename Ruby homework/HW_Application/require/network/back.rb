#!/usr/bin/ruby -w

# Back-end part of application

require "socket"
require "json"
require_relative "utility.rb"


#===============================================================================
# Server
#===============================================================================

class Server_QA
    attr_reader :config
    attr_accessor :do_log


    def initialize(configPath, do_log = false)
        @config = load_configuration(configPath)
        @do_log = do_log
        if @do_log
            puts "Loaded configuration:"
            @config.each_pair do |key, value|
                puts "  #{key}: #{value}"
            end
        end
    end


    def listen(handler)
        exit_flag = false
        server = TCPServer.open(@config["port"])
        puts "Server is running" if @do_log
        while !exit_flag do
            newClient = server.accept

            Thread.start(newClient) do |client|
                i = 0
                loop do
                    q = JSON.parse client.gets
                    # q = {"response": str}
                    puts "Q: #{q}" if @do_log
                    a = handler.call(q, i)
                    # a = {"response": str, "do_stop": bool, "do_close": bool}
                    puts "A: #{a}" if @do_log
                    i += 1

                    if a["do_stop"]
                        exit_flag = true
                        client.puts a.to_json
                        break
                    elsif a["do_close"]
                        client.puts a.to_json
                        break
                    else
                        client.puts a["response"].to_json
                    end
                end
            end

            # Need to initiate self connection to stop server.accept from listening
            TCPSocket.open(@config["hostname"], @config["port"]).close if exit_flag
        end
        server.close
    end
end

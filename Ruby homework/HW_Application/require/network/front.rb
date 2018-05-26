#!/usr/bin/ruby -w

# Front-end part of application

require "socket"
require "json"
require_relative "utility.rb"


#===============================================================================
# Client
#===============================================================================

class Client_QA
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


    def connect(initial_message, handler)
        server = TCPSocket.open(@config["hostname"], @config["port"])
        puts "Connection established" if @do_log

        q = {:response => initial_message}
        # q = {"response": str}
        puts "Q: #{q}" if @do_log
        server.puts q.to_json

        i = 1
        loop do
            a = JSON.parse server.gets
            # a = {"response": str, "do_stop": bool, "do_close": bool}
            puts "A: #{a}" if @do_log

            break if a["do_stop"] || a["do_close"]

            q = handler.call(a, i)
            # q = {"response": str}
            puts "Q: #{q}" if @do_log
            server.puts JSON.generate(q)
            i += 1
        end
        server.close
    end
end

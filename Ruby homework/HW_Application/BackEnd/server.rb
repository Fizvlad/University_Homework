#!/usr/bin/ruby -w

# Back-end part of application

require "socket"
require "./require/utility.rb"

#===============================================================================
# Setting up constants
#===============================================================================

CONFIG = load_configuration

puts "Loaded config:"
CONFIG.each_pair { |key, value|
    puts "  #{key}: #{value}"
}


#===============================================================================
# Main
#===============================================================================

exitFlag = false

server = TCPServer.open(CONFIG["port"])
loop {
    break if exitFlag

    newClient = server.accept
    Thread.start(newClient) { |client|
        auth = client.gets.strip
        if auth == CONFIG["auth"]
            client.puts "auth_success"

            command = client.gets.strip
            puts "Requested command: #{command}"
            case command
            when "exit", "quit"
                exitFlag = true
                # Need to initiate self connection to stop server.accept from listening
                TCPSocket.open(CONFIG["hostname"], CONFIG["port"]).close
            else
                client.puts "unknown_command"
            end

        else
            client.puts "auth_fail wrong_key"
            puts "Unsuccessful authorization atempt. Used keyword: #{auth}"
        end
        client.close
    }
}
server.close

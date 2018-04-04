#!/usr/bin/ruby -w

# Front-end part of application

require "socket"
require "./require/utility.rb"

#===============================================================================
# Setting up constants
#===============================================================================

CONFIG = load_configuration


#===============================================================================
# Main
#===============================================================================

server = TCPSocket.open(CONFIG["hostname"], CONFIG["port"])

server.puts(CONFIG["auth"])
authResult = server.gets.strip
if authResult == "auth_success"
    server.puts "exit"
else
    reason = server.gets.strip
    puts "Failed authentification. Reason: #{reason}"
end

server.close

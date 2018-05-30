require "socket"
require "json"

require "openssl"
require "base64"

require_relative "utility.rb"

module Back_Front__shared
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


    def send(other, obj, rsa = nil)
        str = JSON.generate(obj)
        str = Base64::strict_encode64(rsa.public_encrypt(str)) unless rsa.nil?
        other.puts(str)
    end

    def receive(other, rsa = nil)
        str = other.gets
        str = rsa.private_decrypt(Base64::decode64(str)) unless rsa.nil?
        return JSON.parse(str)
    end
end

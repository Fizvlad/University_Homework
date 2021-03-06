#!/usr/bin/ruby -w

# Utility functions for network front/end

#===============================================================================
# Functions
#===============================================================================

# Loads configuration file and returnes map of key-value
def load_configuration(configPath)
    config = Hash.new
    File.open(configPath, "r") { |file|
        for line in file
            value = line.strip.split
            next if value.count == 0
            key = value.delete_at(0)
            value = value[0] if value.count == 1

            config[key] = value
        end
    }
    return config
end

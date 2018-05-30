class BMP

    attr_reader :width, :height, :pixels


    def initialize(*args)
        case args.length
        when 1
            path = File.absolute_path(args[0])
            load_from_file(path)
        when 2, 3
            raise(ArgumentError, "Wrong argument") unless args[0].is_a?(Numeric) && args[0] > 0 &&
                                                          args[1].is_a?(Numeric) && args[1] > 0 &&
                                                          (args.length == 2 || args[2].to_s.match?(/[0-1A-F]{6}/))
            initialize_new(args[0], args[1], args.length == 2 ? "000000" : args[2].to_s)
        else
            raise(ArgumentError, "Wrong amount of arguments: #{args.length}")
        end
    end


    def to_s()
        "BMP image #{@width}x#{@height}. Size: #{file_size()}b"
    end


    def [](index_y, index_x)
        @pixels[index_y][index_x]
    end

    def []=(index_y, index_x, value)
        @pixels[index_y][index_x] = value
    end


    def save_as(path)
        File.open(path, "wb") { |file|
            write_header(file)
            write_info(file)
            write_bitmap(file)
        }
    end


    private


    def load_from_file(path)
        # TODO Reading from file

    end

    def initialize_new(width, height, default_color)
        @width = width
        @height = height
        @pixels = Array.new(@height) { Array.new(@width) { default_color } }
    end


    def file_size()
        bitmap_offset() + 4 * @width * @height
    end

    def header_size()
        14
    end

    def info_size()
        40
    end

    def bitmap_offset()
        header_size() + info_size()
    end


    def write_header(file)
        file.pos = 0

        bfType      = "BM"
        bfSize      = file_size()
        bfReserved1 = 0
        bfReserved2 = 0
        bfOffBit    = bitmap_offset()

        directive = "A2LSSL" # S = WORD, L = DWORD. A2 used for string

        file << [bfType, bfSize, bfReserved1, bfReserved2, bfOffBit].pack(directive)
    end

    def write_info(file)
        file.pos = header_size()

        biSize = info_size()
        biWidth = @width
        biHeight = -1 * @height
        biPlanes = 1
        biBitCount = 24 # TODO bit_count
        biCompression = 0
        biSizeImage = 0
        biXPelsPerMeter = 0
        biYPelsPerMeter = 0
        biClrUsed = 0
        biClrImportant = 0

        directive = "LllSSLLllLL" # S = WORD, L = DWORD, l = LONG

        file << [biSize, biWidth, biHeight, biPlanes, biBitCount, biCompression, biSizeImage, biXPelsPerMeter, biYPelsPerMeter, biClrUsed, biClrImportant].pack(directive)
    end

    def write_bitmap(file)
        file.pos = bitmap_offset()

        @pixels.each { |line|
            line.each { |pixel|
                r = pixel[0, 2].hex
                g = pixel[2, 2].hex
                b = pixel[4, 2].hex
                directive = "CCC"

                file << [b, g, r].pack(directive)
            }
            file << Array.new(line.length % 4, 0).pack("C#{line.length % 4}") # Line must be filled with 0 so length % 4 = 0
        }
    end

end

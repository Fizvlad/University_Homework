class BMP
    # TODO Current version only support V3 header and 24 bit count

    attr_reader :width, :height, :file_size

    def initialize(*args)
        case args.length
        when 1
            path = File.absolute_path(args[0])
            initialize_from_file(path)
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
        "BMP image #{@width}x#{@height}. Size: #{@file_size}b"
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


    def initialize_from_file(path)
        @width = nil
        @height = nil
        @pixels = nil

        @if_first_line_at_top = nil

        @header_size = 14
        @info_size = nil
        @bitcount = nil
        @bitmap_offset = nil

        @file_size = nil

        File.open(path, "rb") { |file|
            read_header(file)
            read_info(file)
            read_bitmap(file)
        }
    end

    def initialize_new(width, height, default_color)
        @width = width
        @height = height
        @pixels = Array.new(@height) { Array.new(@width) { default_color } }

        @if_first_line_at_top = true

        @header_size = 14
        @info_size = 40
        @bitcount = 24
        @bitmap_offset = @header_size + @info_size

        @file_size = @bitmap_offset + 4 * @width * @height
    end


    def write_header(file)
        file.pos = 0

        bfType      = "BM"
        bfSize      = @file_size
        bfReserved1 = 0
        bfReserved2 = 0
        bfOffBit    = @bitmap_offset

        directive = "A2LSSL" # S = WORD, L = DWORD. A2 used for string

        file << [bfType, bfSize, bfReserved1, bfReserved2, bfOffBit].pack(directive)
    end

    def write_info(file)
        file.pos = @header_size

        biSize = @info_size
        biWidth = @width
        biHeight = (@if_first_line_at_top ? -1 : 1) * @height
        biPlanes = 1
        biBitCount = @bitcount
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
        file.pos = @bitmap_offset

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


    def util_read(file, size, directive, errorMessage)
        str = file.read(size)
        raise(ArgumentError, errorMessage) if str.nil? || str.length < size
        return str.unpack(directive)
    end

    def util_form_color_str(r, g, b)
        result = ""
        [r, g, b].each { |elem|
            elem = ("%x" % elem).upcase
            elem = "0" + elem if elem.length < 2

            result += elem
        }
        result
    end

    def read_header(file)
        file.pos = 0

        directive = "A2LSSL" # S = WORD, L = DWORD. A2 used for string
        arr = util_read(file, @header_size, directive, "Can't open file as BMP image. Bad header")

        raise(ArgumentError, "Can't open file as BMP image. Bad header. No \"BM\" prefix") if arr[0] != "BM"
        @file_size = arr[1]
        raise(ArgumentError, "Can't open file as BMP image. Bad header. Worng value of reserved fields") if arr[2] != 0 || arr[3] != 0
        @bitmap_offset = arr[4]
    end

    def read_info(file)
        file.pos = @header_size

        @info_size = util_read(file, 4, "L", "Can't open file as BMP image. Bad info")[0]
        raise(ArgumentError, "Unable to work with BMP file of this version") if @info_size != 40

        directive = "llSSLLllLL" # S = WORD, L = DWORD, l = LONG
        arr = util_read(file, @info_size - 4, directive, "Can't open file as BMP image. Bad info")

        @width = arr[0]
        @if_first_line_at_top = arr[1] < 0
        @height = arr[1].abs
        raise(ArgumentError, "Can't open file as BMP image. Bad info. Unexcepted biPlanes value") if arr[2] != 1
        @bitcount = arr[3]
        raise(ArgumentError, "Unable to work with not 24 bitcount") if @bitcount != 24
        raise(ArgumentError, "Can't open file as BMP image. Bad info. Unexcepted biCompression value") if arr[4] != 0
        #raise(ArgumentError, "Can't open file as BMP image. Bad info. Unexcepted biSizeImage value") if arr[5] != 0
        #raise(ArgumentError, "Can't open file as BMP image. Bad info. Unexcepted biXPelsPerMeter value") if arr[6] != 0
        #raise(ArgumentError, "Can't open file as BMP image. Bad info. Unexcepted biYPelsPerMeter value") if arr[7] != 0
        raise(ArgumentError, "Can't open file as BMP image. Bad info. Unexcepted biClrUsed value") if arr[8] != 0
        raise(ArgumentError, "Can't open file as BMP image. Bad info. Unexcepted biClrImportant value") if arr[9] != 0
    end

    def read_bitmap(file)
        file.pos = @bitmap_offset

        @pixels = Array.new(@height) { Array.new(@width) { nil } }

        @pixels.each_index { |line_index|
            @pixels[line_index].each_index { |pixel_index|
                directive = "CCC"
                arr = util_read(file, 3, directive, "Can't open file as BMP image. Bad bitmap")
                r = arr[2]
                g = arr[1]
                b = arr[0]
                @pixels[line_index][pixel_index] = util_form_color_str(r, g, b)
            }
            file.read(@pixels[line_index].length % 4) # Line must be filled with 0 so length % 4 = 0
        }
    end

end

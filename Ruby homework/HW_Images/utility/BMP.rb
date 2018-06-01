class BMP
    # TODO Current version only support V3 header and 24 bit count

    attr_reader :info_version, :bitcount, :file_size

    def initialize(*args)
        @pixels = nil
        @if_first_line_at_top = nil
        @info_version = nil
        @bitcount = nil
        @bitmap_offset = nil
        @file_size = nil

        @header_size = 14
        @possible_info_sizes = [40, 108, 124]

        case args.length
        when 1
            if args[0].is_a?(BMP)
                initialize_copy(args[0])
            else
                initialize_from_file(File.absolute_path(args[0]))
            end
        when 2, 3
            raise(ArgumentError, "Wrong argument") unless util_check_unsigned_integer(args[0]) &&
                                                          util_check_unsigned_integer(args[1]) &&
                                                          (args.length == 2 || util_check_color_str(args[2]))
            initialize_new(args[0], args[1], args.length == 2 ? "000000" : args[2].to_s)
        else
            raise(ArgumentError, "Wrong amount of arguments: #{args.length}")
        end
    end


    def width
        return nil if @pixels.nil? || @pixels[0].nil?
        @pixels[0].length
    end

    def height
        return nil if @pixels.nil?
        @pixels.length
    end


    def to_s
        "BMP image #{width}x#{height}. Size: #{@file_size}b"
    end


    def [](index_x, index_y)
        @pixels[index_y][index_x]
    end

    def []=(index_x, index_y, value)
        @pixels[index_y][index_x] = value
    end


    def save_as(path)
        File.open(path, "wb") { |file|
            write_header(file)
            write_info(file)
            write_bitmap(file)
        }
    end


    def resize!(w, h, power = 2)
        raise(ArgumentError, "Wrong argument") unless util_check_unsigned_integer(w) &&
                                                      util_check_unsigned_integer(h) &&
                                                      power.is_a?(Numeric) && power >= 0

        h_c = width.to_f  / w
        v_c = height.to_f / h

        new_pixels = Array.new(h) { Array.new(w) { nil } }
        new_pixels.each_index { |line_index|
            new_pixels[line_index].each_index { |pixel_index|
                precision = 3

                x_position = (pixel_index + 0.5) * h_c
                x_index = x_position.round
                x_fractional = (x_position - x_index).round(precision)

                y_position = (line_index + 0.5) * v_c
                y_index = y_position.round
                y_fractional = (y_position - y_index).round(precision)

                color11 = x_index < 1 || y_index < 1 ? nil : @pixels&.at(y_index - 1)&.at(x_index - 1);
                color12 = y_index < 1 ? nil : @pixels&.at(y_index - 1)&.at(x_index);
                color21 = x_index < 1 ? nil : @pixels&.at(y_index)&.at(x_index - 1);
                color22 = @pixels&.at(y_index)&.at(x_index);
                new_pixels[line_index][pixel_index] = util_blend_2d(color11, color12, color21, color22, (x_fractional + 1)/2, (y_fractional + 1)/2, power)
            }
        }

        @pixels = new_pixels
        self
    end

    def resize(w, h, power = 2)
        BMP.new(self).resize!(w, h, power)
    end


    protected

    attr_accessor :pixels, :if_first_line_at_top, :bitmap_offset


    private

    # ##########################################################################
    # Initialize
    # ##########################################################################

    def initialize_from_file(path)
        File.open(path, "rb") { |file|
            read_header(file)
            read_info(file)
            read_bitmap(file)
        }
    end

    def initialize_new(w, h, default_color)
        @pixels = Array.new(h) { Array.new(w) { default_color } }

        @if_first_line_at_top = true

        @info_version = 3
        @bitcount = 24
        @bitmap_offset = @header_size + @possible_info_sizes[@info_version - 3]

        @file_size = @bitmap_offset + 4 * width * height
    end

    def initialize_copy(other)
        @pixels = Array.new(other.height) { Array.new(other.width) { nil } }

        @pixels.each_index { |line_index|
            @pixels[line_index].each_index { |pixel_index|
                @pixels[line_index][pixel_index] = other.pixels[line_index][pixel_index]
            }
        }

        @if_first_line_at_top = other.if_first_line_at_top

        @info_version = other.info_version
        @bitcount = other.bitcount
        @bitmap_offset = other.bitmap_offset

        @file_size = other.file_size
    end

    # ##########################################################################
    # Saving
    # ##########################################################################

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

        biSize = @possible_info_sizes[@info_version - 3]
        biWidth = width
        biHeight = (@if_first_line_at_top ? -1 : 1) * height
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


    # ##########################################################################
    # Reading
    # ##########################################################################

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

        info_version_i = @possible_info_sizes.find_index(util_read(file, 4, "L", "Can't open file as BMP image. Bad info")[0])
        raise(ArgumentError, "Can't open file as BMP image. Bad info. Unexcepted info size") if info_version_i.nil?
        @info_version = 3 + info_version_i
        raise(ArgumentError, "Unable to work with BMP file of this version") if @info_version != 3

        directive = "llSSLLllLL" # S = WORD, L = DWORD, l = LONG
        arr = util_read(file, @possible_info_sizes[@info_version - 3] - 4, directive, "Can't open file as BMP image. Bad info")

        w = arr[0]
        @if_first_line_at_top = arr[1] < 0
        h = arr[1].abs

        @pixels = Array.new(h) { Array.new(w) { nil } }

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


    # ##########################################################################
    # Utility
    # ##########################################################################

    def util_read(file, size, directive, error_message)
        str = file.read(size)
        raise(ArgumentError, error_message) if str.nil? || str.length < size
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

    def util_check_color_str(str)
        str.match?(/[0-1A-F]{6}/)
    end

    def util_check_unsigned_integer(n)
        n.is_a?(Numeric) && n > 0 && n % 1 == 0
    end

    def util_blend_1d(color1, color2, i = 0, power = 2)
        return nil if color1.nil? && color2.nil?
        return color1 if color2.nil? || i == 0
        return color2 if color1.nil? || i == 1
        r1 = color1[0, 2].hex
        g1 = color1[2, 2].hex
        b1 = color1[4, 2].hex

        r2 = color2[0, 2].hex
        g2 = color2[2, 2].hex
        b2 = color2[4, 2].hex

        if power == 0
            i = 0.5
        else
            i = ((2*i - 1).abs**(1/power) * (i < 0.5 ? -1 : 1)) / 2 + 0.5
        end
        r = r1 * (1 - i) + r2 * i
        g = g1 * (1 - i) + g2 * i
        b = b1 * (1 - i) + b2 * i

        util_form_color_str(r, g, b)
    end

    def util_blend_2d(color11, color12, color21, color22, i_x = 0.5, i_y = 0.5, power = 2)
        color_top  = i_y == 1 ? "888888" : util_blend_1d(color11, color12, i_x, power)
        color_down = i_y == 0 ? "888888" : util_blend_1d(color21, color22, i_x, power)

        util_blend_1d(color_top, color_down, i_y, power)
    end

end

#!/usr/bin/ruby -w

require_relative "utility/BMP.rb"

bmp = BMP.new(5, 5, "FFFFFF")

bmp[0, 0] = "000000"
bmp[0, 1] = "FF0000"
bmp[0, 2] = "00FF00"
bmp[0, 3] = "0000FF"
bmp[3, 0] = "000000"
bmp[3, 1] = "FF0000"
bmp[3, 2] = "00FF00"
bmp[3, 3] = "0000FF"

puts bmp

bmp.save_as("img.bmp")

bmp2 = BMP.new("img.bmp")
puts bmp2

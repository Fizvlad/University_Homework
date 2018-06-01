#!/usr/bin/ruby -w

require_relative "utility/BMP.rb"

bmp = BMP.new("img.bmp")

puts bmp
bmp.save_as("init.bmp")

bmp.resize(1000, 1000).save_as("img_big.bmp")

puts bmp

bmp.resize(100, 100).save_as("img_small.bmp")

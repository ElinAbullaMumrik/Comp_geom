cmake_minimum_required(VERSION 3.17)
project(lab4)

set(CMAKE_CXX_STANDARD 14)

add_executable(lab4 main.cpp PPM.h Converter.h pixel_t.h converters/fromRGBtoHSL.h converters/fromRGBtoHSV.h converters/BaseConverter.h converters/fromRGBtoYCbCr601.h converters/fromRGBtoYCbCr709.h converters/fromRGBtoYCoCg.h converters/fromRGBtoCMY.h converters/fromCMYtoRGB.h converters/fromYCoCgtoRGB.h converters/fromHSLtoRGB.h converters/fromHSVtoRGB.h converters/fromYCbCr601toRGB.h converters/fromYCbCr709toRGB.h)
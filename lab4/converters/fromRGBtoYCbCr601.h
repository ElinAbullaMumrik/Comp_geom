//
// Created by elin on 14.06.20.
//

#ifndef LAB4_FROMRGBTOYCBCR601_H
#define LAB4_FROMRGBTOYCBCR601_H

#include "../Converter.h"

class fromRGBtoYCbCr601 : public Converter {
    pixel_t convert(pixel_t pixel) override {
        double R = pixel.R / 255.;
        double G = pixel.G / 255.;
        double B = pixel.B / 255.;

        double KR = 0.299;
        double KG = 0.587;
        double KB = 0.114;

        double Y = KR * R + KG * G + KB * B;
        double PB = 1 / 2. * (B - Y) / (1.0 - KB);
        double PR = 1 / 2. * (R - Y) / (1.0 - KR);
        pixel.R = (unsigned char) (std::max(std::min(255., round(Y * 255)), 0.));
        pixel.G = (unsigned char) (std::max(std::min(255., round((PB + 0.5) * 255)), 0.));
        pixel.B = (unsigned char) (std::max(std::min(255., round((PR + 0.5) * 255)), 0.));
        return pixel;
    }
};


#endif //LAB4_FROMRGBTOYCBCR601_H

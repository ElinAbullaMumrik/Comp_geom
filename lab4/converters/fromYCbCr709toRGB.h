//
// Created by elin on 14.06.20.
//

#ifndef LAB4_FROMYCBCR709TORGB_H
#define LAB4_FROMYCBCR709TORGB_H

#include "../Converter.h"

class fromYCbCr709toRGB : public Converter{
    pixel_t convert(pixel_t pixel) override {
        double Y = pixel.R / 255.;
        double CB = pixel.G / 255. - 0.5;
        double CR = pixel.B / 255. - 0.5;
        double KR = 0.2126;
        double KG = 0.7152;
        double KB = 0.0722;
        double r = Y + (2.0 - 2.0 * KR) * CR;
        double g = Y - KB * (2.0 - 2.0 * KB) * CB / KG - KR * (2 - 2.0 * KR) * CR / KG;
        double b = Y + (2.0 - 2.0 * KB) * CB;
        pixel.R = (unsigned char) (std::max(std::min(255., round(r * 255)), 0.));
        pixel.G = (unsigned char) (std::max(std::min(255., round(g * 255)), 0.));
        pixel.B = (unsigned char) (std::max(std::min(255., round(b * 255)), 0.));
        return pixel;
    }
};


#endif //LAB4_FROMYCBCR709TORGB_H

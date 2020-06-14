//
// Created by elin on 14.06.20.
//

#ifndef LAB4_FROMRGBTOYCOCG_H
#define LAB4_FROMRGBTOYCOCG_H

#include "../Converter.h"

class fromRGBtoYCoCg : public Converter {
    pixel_t convert(pixel_t pixel) override {
        double r = pixel.R * 1.0 / 255;
        double g = pixel.G * 1.0 / 255;
        double b = pixel.B * 1.0 / 255;
        double y = r / 4 + g / 2 + b / 4;
        double co = r / 2 - b / 2 + 0.5;
        double cg = -r / 4 + g / 2 - b / 4 + 0.5;
        pixel.R = (unsigned char) (std::max(std::min(255., round(y * 255)), 0.));
        pixel.G = (unsigned char) (std::max(std::min(255., round(co * 255)), 0.));
        pixel.B = (unsigned char) (std::max(std::min(255., round(cg * 255)), 0.));

        return pixel;
    }

};


#endif //LAB4_FROMRGBTOYCOCG_H

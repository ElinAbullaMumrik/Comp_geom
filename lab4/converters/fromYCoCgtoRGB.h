//
// Created by elin on 14.06.20.
//

#ifndef LAB4_FROMYCOCGTORGB_H
#define LAB4_FROMYCOCGTORGB_H

#include "../Converter.h"

class fromYCoCgtoRGB : public Converter {
    pixel_t convert(pixel_t pixel) override {
        double y = pixel.R * 1.0 / 255;
        double co = pixel.G * 1.0 / 255 - 0.5;
        double cg = pixel.B * 1.0 / 255 - 0.5;
        double r = y + co - cg;
        double g = y + cg;
        double b = y - co - cg;
        pixel.R = (unsigned char) (std::max(std::min(255., round(r * 255)), 0.));
        pixel.G = (unsigned char) (std::max(std::min(255., round(g * 255)), 0.));
        pixel.B = (unsigned char) (std::max(std::min(255., round(b * 255)), 0.));
        return pixel;
    }
};


#endif //LAB4_FROMYCOCGTORGB_H

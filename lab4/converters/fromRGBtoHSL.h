//
// Created by elin on 14.06.20.
//

#ifndef LAB4_FROMRGBTOHSL_H
#define LAB4_FROMRGBTOHSL_H

#include <algorithm>
#include <cmath>
#include "../Converter.h"

class fromRGBtoHSL : public Converter {
    pixel_t convert(pixel_t pixel) override {
        double r = pixel.R / 255.;
        double g = pixel.G / 255.;
        double b = pixel.B / 255.;
        double maximum = std::max(std::max(b, g), r);
        double minimum = std::min(std::min(r, g), b);
        double H, S, L;
        if (maximum == r && g >= b)
            H = 60 * (g - b) / (maximum - minimum);
        if (maximum == r && b < g)
            H = 60 * (g - b) / (maximum - minimum) + 360;
        if (maximum == g)
            H = 60 * (b - r) / (maximum - minimum) + 120;
        if (maximum == b)
            H = 60 * (r - g) / (maximum - minimum) + 240;
        L = 1. / 2. * (maximum + minimum);
        S = (maximum - minimum) / (1 - std::abs(1 - maximum - minimum));
        pixel.R = (unsigned char) round(H / 360 * 255);
        pixel.G = (unsigned char) round(S * 255);
        pixel.B = (unsigned char) round(L * 255);
        return pixel;
    }
};


#endif //LAB4_FROMRGBTOHSL_H

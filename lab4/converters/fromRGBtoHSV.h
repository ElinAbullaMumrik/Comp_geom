//
// Created by elin on 14.06.20.
//

#ifndef LAB4_FROMRGBTOHSV_H
#define LAB4_FROMRGBTOHSV_H


#include "../Converter.h"

class fromRGBtoHSV : public Converter {
    pixel_t convert(pixel_t pixel) override {
        double r = pixel.R / 255.;
        double g = pixel.G / 255.;
        double b = pixel.B / 255.;
        double maximum = std::max(std::max(b, g), r);
        double minimum = std::min(std::min(r, g), b);
        double H, S;
        double V = maximum;
        double c = maximum - minimum;

        if (c == 0)
            H = 0;
        else if (V == g)
            H = 2 + (b - r) / c;
        else if (V == r)
            H = (g - b) / c;
        else
            H = 4 + (r - g) / c;
        H *= 60.;
        if (H < 0)
            H += 360;

        if (V == 0) {
            S = 0;
        } else {
            S = c / V;
        }
        pixel.R = (unsigned char) round(H / 360. * 255.);
        pixel.G = (unsigned char) round(S * 255.);
        pixel.B = (unsigned char) round(V * 255.);

        return pixel;
    }
};


#endif //LAB4_FROMRGBTOHSV_H

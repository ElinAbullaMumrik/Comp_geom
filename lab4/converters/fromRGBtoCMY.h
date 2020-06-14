//
// Created by elin on 14.06.20.
//

#ifndef LAB4_FROMRGBTOCMY_H
#define LAB4_FROMRGBTOCMY_H

#include "../Converter.h"

class fromRGBtoCMY : public Converter {
    pixel_t convert(pixel_t pixel) override {
        int R = pixel.R;
        int G = pixel.G;
        int B = pixel.B;
        pixel.R = (unsigned char) (round((1 - (double) R / 255. * 255)));
        pixel.G = (unsigned char) (round((1 - (double) G / 255. * 255)));
        pixel.B = (unsigned char) (round((1 - (double) B / 255. * 255)));

        return pixel;
    }
};


#endif //LAB4_FROMRGBTOCMY_H

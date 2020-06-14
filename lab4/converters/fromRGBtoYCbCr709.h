//
// Created by elin on 14.06.20.
//

#ifndef LAB4_FROMRGBTOYCBCR709_H
#define LAB4_FROMRGBTOYCBCR709_H

#include "../Converter.h"

class fromRGBtoYCbCr709 : public Converter{
    pixel_t convert(pixel_t pixel) override{
        return pixel;
    }
};


#endif //LAB4_FROMRGBTOYCBCR709_H

//
// Created by elin on 14.06.20.
//

#ifndef LAB4_FROMRGBTOYCBCR601_H
#define LAB4_FROMRGBTOYCBCR601_H
#include "../Converter.h"

class fromRGBtoYCbCr601 : public Converter{
    pixel_t convert(pixel_t pixel) override{
        return pixel;
    }
};


#endif //LAB4_FROMRGBTOYCBCR601_H

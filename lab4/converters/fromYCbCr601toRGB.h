//
// Created by elin on 14.06.20.
//

#ifndef LAB4_FROMYCBCR601TORGB_H
#define LAB4_FROMYCBCR601TORGB_H

#include "../Converter.h"

class fromYCbCr601toRGB : public Converter{
    pixel_t convert(pixel_t pixel) override{
        return pixel;
    }
};


#endif //LAB4_FROMYCBCR601TORGB_H

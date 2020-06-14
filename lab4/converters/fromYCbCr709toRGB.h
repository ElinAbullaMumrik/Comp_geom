//
// Created by elin on 14.06.20.
//

#ifndef LAB4_FROMYCBCR709TORGB_H
#define LAB4_FROMYCBCR709TORGB_H

#include "../Converter.h"

class fromYCbCr709toRGB : public Converter{
    pixel_t convert(pixel_t pixel) override{
        return pixel;
    }
};


#endif //LAB4_FROMYCBCR709TORGB_H

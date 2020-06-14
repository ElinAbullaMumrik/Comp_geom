//
// Created by elin on 14.06.20.
//

#ifndef LAB4_FROMYCOCGTORGB_H
#define LAB4_FROMYCOCGTORGB_H
#include "../Converter.h"

class fromYCoCgtoRGB : public Converter{
    pixel_t convert(pixel_t pixel) override{
        return pixel;
    }
};


#endif //LAB4_FROMYCOCGTORGB_H

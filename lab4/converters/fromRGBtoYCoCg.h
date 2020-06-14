//
// Created by elin on 14.06.20.
//

#ifndef LAB4_FROMRGBTOYCOCG_H
#define LAB4_FROMRGBTOYCOCG_H

#include "../Converter.h"

class fromRGBtoYCoCg : public Converter{
    pixel_t convert(pixel_t pixel) override{
        return pixel;
    }
};


#endif //LAB4_FROMRGBTOYCOCG_H

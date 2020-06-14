//
// Created by elin on 14.06.20.
//

#ifndef LAB4_CONVERTER_H
#define LAB4_CONVERTER_H

#include "pixel_t.h"

class Converter {
public:
    virtual pixel_t convert(pixel_t pixel)=0;
};


#endif //LAB4_CONVERTER_H

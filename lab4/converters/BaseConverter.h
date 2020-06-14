//
// Created by elin on 14.06.20.
//

#ifndef LAB4_BASECONVERTER_H
#define LAB4_BASECONVERTER_H


#include "../Converter.h"

class BaseConverter : public Converter{
    pixel_t convert(pixel_t pixel)override {
        return pixel;
    }
};


#endif //LAB4_BASECONVERTER_H

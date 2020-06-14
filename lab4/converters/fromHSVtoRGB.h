//
// Created by elin on 14.06.20.
//

#ifndef LAB4_FROMHSVTORGB_H
#define LAB4_FROMHSVTORGB_H
#include "../Converter.h"

class fromHSVtoRGB : public Converter{
    pixel_t convert(pixel_t pixel) override{

        double H = pixel.R / 255.0 * 360.0;
        double S = pixel.G / 255.0;
        double V = pixel.B / 255.0;
        double c = V * S;
        double x = c * (1 - abs(((int) (H/60.)) % 2 + ((H/60.) - (int)(H/60.)) - 1));
        double m = V - c;
        double r, g, b;
        if(H >= 0 && H <= 60)
            r = c, g = x, b = 0;
        else if(H >= 60 && H <= 120)
            r = x, g = c, b = 0;
        else if(H >= 120 && H <= 180)
            r = 0, g = c, b = x;
        else if(H >= 180 && H <= 240)
            r = 0, g = x, b = c;
        else if(H >= 240 && H <= 300)
            r = x, g = 0, b = c;
        else
            r = c, g = 0, b = x;

        int r_int = (int)(round((r + m) * 255));
        int g_int = (int)(round((g + m) * 255));
        int b_int = (int)(round((b + m) * 255));

        if(r_int < 0) r_int = 0;
        if(r_int > 255) r_int = 255;
        if(b_int < 0) b_int = 0;
        if(b_int > 255) b_int = 255;
        if(g_int < 0) g_int = 0;
        if(g_int > 255) g_int = 255;
        pixel.R = (unsigned char)r_int;
        pixel.G = (unsigned char)g_int;
        pixel.B = (unsigned char)b_int;
        return pixel;
    }
};


#endif //LAB4_FROMHSVTORGB_H

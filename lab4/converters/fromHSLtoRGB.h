//
// Created by elin on 14.06.20.
//

#ifndef LAB4_FROMHSLTORGB_H
#define LAB4_FROMHSLTORGB_H

#include "../Converter.h"

class fromHSLtoRGB : public Converter {
    pixel_t convert(pixel_t pixel) override {

        double h = pixel.R / 255.0;
        double s = pixel.G / 255.0;
        double l = pixel.B / 255.0;
        double q, p;
        if (l < 0.5)
            q = l * (s + 1.0);
        else
            q = l + s - (l * s);
        p = l * 2 - q;
        double tr = h + 1.0 / 3.0, tg = h, tb = h - 1.0 / 3.0;
        if (tr < 0) tr += 1.0;
        if (tr > 1.0) tr -= 1.0;
        if (tg < 0) tg += 1.0;
        if (tg > 1.0) tg -= 1.0;
        if (tb < 0) tb += 1.0;
        if (tb > 1.0) tb -= 1.0;
        double r, g, b;

        if (tr < 1.0 / 6.0)
            r = p + ((q - p) * 6.0 * tr);
        else if (tr >= 1.0 / 6.0 && tr < 0.5)
            r = q;
        else if (tr >= 0.5 && tr < 2.0 / 3.0)
            r = p + ((q - p) * (2.0 / 3.0 - tr) * 6.0);
        else
            r = p;

        if (tg < 1.0 / 6.0)
            g = p + ((q - p) * 6.0 * tg);
        else if (tg >= 1.0 / 6.0 && tg < 0.5)
            g = q;
        else if (tg >= 0.5 && tg < 2.0 / 3.0)
            g = p + ((q - p) * (2.0 / 3.0 - tg) * 6.0);
        else
            g = p;

        if (tb < 1.0 / 6.0)
            b = p + ((q - p) * 6.0 * tb);
        else if (tb >= 1.0 / 6.0 && tb < 0.5)
            b = q;
        else if (tb >= 0.5 && tb < 2.0 / 3.0)
            b = p + ((q - p) * (2.0 / 3.0 - tb) * 6.0);
        else
            b = p;
        pixel.R = (unsigned char)round(r*255);
        pixel.G = (unsigned char)round(g*255);
        pixel.B = (unsigned char)round(b*255);


        return pixel;
    }
};


#endif //LAB4_FROMHSLTORGB_H

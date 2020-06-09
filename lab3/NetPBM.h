//
// Created by elin on 07.06.20.
//
#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <set>

#ifndef LAB3_NETPBM_H
#define LAB3_NETPBM_H

using namespace std;
struct point_t {
    double x;
    double y;
};

class NetPBM {
private:
    ifstream *file;
    uint16_t type = -1;
    uint16_t width = -1;
    uint16_t height = -1;
    uint16_t depth = -1;
    unsigned char **array;

    void read_header() {
        unsigned char buf;
        *this->file >> buf;
        if (buf != 'P') {
            throw logic_error("Bad file");
        }
        *this->file >> buf;
        if (buf == '1' || buf == '2' || buf == '3' || buf == '4' || buf == '6' || buf == '7') {
            throw logic_error("Not supported type of NetPCM");
        } else if (buf == '5')
            this->type = 5;
        else {
            throw logic_error("Bad file");
        }
        *this->file >> buf;
        *this->file >> this->width;
        *this->file >> buf;
        *this->file >> this->height;
        *this->file >> buf;
        *this->file >> this->depth;
        if (this->depth != 255) {
            throw logic_error("Not supported non 255 colors count");
        }
    }

    void read_data() {
        for (int i = 0; i < this->height; i++) {
            for (int j = 0; j < this->width; j++) {
                *this->file >> this->array[i][j];
            }
        }
    }

// https://en.wikipedia.org/wiki/SRGB#The_sRGB_transfer_function_(%22gamma%22)
    static double gammasRGB(double u) {
        if (u <= 0.0031308) {
            return 323 * u / 25.;
        } else {
            return (211. * pow(u, 5. / 12.) - 11.) / 200.;
        }
    }

    static double ungammasRGB(double u) {
        if (u <= 0.04045) {
            return 25 * u / 323;
        } else {
            return pow((200. * u + 11.) / 211., 12. / 5.);
        }
    }

    static double gamma(double u, double gamma) {
        return pow(u, 1.0 / gamma);
    }

    static double ungamma(double u, double gamma) {
        return pow(u, gamma);
    }

    void draw_point(int x, int y, double brightness, unsigned char color, double gamma_value) {
        if (!(x < width && y < height && x >= 0 && y >= 0 && color >= 0 && color <= depth))
            throw invalid_argument("Invalid argument");
        double value = ((double) this->array[y][x]) / this->depth;
        double color_value = ((double) color) / (double) depth;
        if (gamma_value == -1) {
            value = ungammasRGB(value);
            color_value = ungammasRGB(color_value);
        } else {
            value = ungamma(value, gamma_value);
            color_value = ungamma(color_value, gamma_value);
        }
        value = value + brightness * (color_value - value);
        if (gamma_value == -1) {
            value = gammasRGB(value);
        } else {
            value = gamma(value, gamma_value);
        }
        if (value >= 1 - 1 / 1e9)
            value = 1;
        this->array[y][x] = (unsigned char) round(this->depth * value);
    }

    static void swap(point_t *a, point_t *b) {
        auto *buf = new point_t;
        *buf = *a;
        *a = *b;
        *b = *buf;
        delete buf;
    }

public:
    int16_t getType() const {
        return type;
    }


    void write_to_file(ofstream *outfile) {
        *outfile << "P5" << (unsigned char) (10) << width << " " << height << (unsigned char) (10) << depth
                 << (unsigned char) (10);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                *outfile << array[i][j];
            }
        }
    }

    unsigned char findNearestPaletteColor(unsigned char color, int bitness) {
        unsigned char tmp = color & (((1u << bitness) - 1) << (8 - bitness));
        color = 0;
        for (unsigned i = 0; i < 8 / bitness + 1; ++i) {
            color = color | ((unsigned char) (tmp >> bitness * i));
        }
        return color;
    }

    void no_dithering(double gamma_value, int bitness) {
        for (int i = 0; i < this->height; i++) {
            for (int j = 0; j < this->width; j++) {
                char color = findNearestPaletteColor(this->array[i][j], bitness);
                draw_point(j, i, 1, color, gamma_value);
            }
        }
    }

//    void no_dithering(double gamma_value, int bitness) {
//    }

    void ordered_dithering(double gamma_value, int bitness) {
        float resizer = pow(2., (double) bitness) / 255;
        double bayerMatrix[8][8] = {
                {0,  48, 12, 60, 3,  51, 15, 63},
                {32, 16, 44, 28, 35, 19, 47, 31},
                {8,  56, 4,  52, 11, 59, 7,  55},
                {40, 24, 36, 20, 43, 27, 39, 23},
                {2,  50, 14, 62, 1,  49, 13, 61},
                {34, 18, 46, 30, 33, 17, 45, 29},
                {10, 58, 6,  54, 9,  57, 5,  53},
                {42, 26, 38, 22, 41, 25, 37, 21}
        };

        for (auto &i : bayerMatrix) {
            for (double &j : i) {
                j = j / 64 - 0.5;
            }
        }
        for (int i = 0; i < this->height; i++) {
            for (int j = 0; j < this->width; j++) {
                int x = i % 8;
                int y = j % 8;
                unsigned char color = findNearestPaletteColor(this->array[i][j] + resizer * bayerMatrix[x][y], bitness);
                draw_point(j, i, 1, color, gamma_value);
            }
        }
    }


    explicit
    NetPBM(ifstream
           *file) {
        this->file = file;
        this->read_header();
        file->ignore(1);

        this->array = new unsigned char *[height];
        for (int i = 0; i < this->height; i++) {
            this->array[i] = new unsigned char[width];
        }
        read_data();
    }

    ~NetPBM() {
        for (int i = 0; i < height; i++) {
            delete[] array[i];
        }
        delete[] array;
    }
};

#endif //LAB3_NETPBM_H

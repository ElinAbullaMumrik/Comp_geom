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

    void read_data(double gamma_value) {
        for (int i = 0; i < this->height; i++) {
            for (int j = 0; j < this->width; j++) {
                unsigned char tmp;
                *this->file >> tmp;

                double value = ((double) tmp) / this->depth;
                if (gamma_value == 0) {
                    value = ungammasRGB(value);
                } else {
                    value = ungamma(value, gamma_value);
                }
                this->array[i][j] = (unsigned char) (value * this->depth);
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

    static double set_gamma(double u, double gamma) {
        return pow(u, 1.0 / gamma);
    }

    static double ungamma(double u, double gamma) {
        return pow(u, gamma);
    }

//    unsigned char read_point(int x, int y, double gamma_value) {
//        if (!(x < width && y < height && x >= 0 && y >= 0))
//            throw invalid_argument("Invalid argument");
//        double value = ((double) this->array[y][x]) / this->depth;
//        if (gamma_value == 0) {
//            value = ungammasRGB(value);
//        } else {
//            value = ungamma(value, gamma_value);
//        }
//        return (unsigned char) round(this->depth * value);
//    }
//
//    void draw_point(int x, int y, unsigned char color, double gamma_value) {
//        if (!(x < width && y < height && x >= 0 && y >= 0 && color >= 0 && color <= depth))
//            throw invalid_argument("Invalid argument");
//        double value;
//        double color_value = color / 255;
//        if (gamma_value == 0) {
//            value = gammasRGB(color_value);
//        } else {
//            value = gamma(color_value, gamma_value);
//        }
//        if (value >= 1 - 1 / 1e9)
//            value = 1;
//        this->array[y][x] = (unsigned char) round(this->depth * value);
//    }
//
//    static void swap(point_t *a, point_t *b) {
//        auto *buf = new point_t;
//        *buf = *a;
//        *a = *b;
//        *b = *buf;
//        delete buf;
//    }

public:
    int16_t getType() const {
        return type;
    }


    void write_to_file(ofstream *outfile, double gamma_value) {
        *outfile << "P5" << (unsigned char) (10) << width << " " << height << (unsigned char) (10) << depth
                 << (unsigned char) (10);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                double value;
                double color_value = (double) array[i][j] / this->depth;
                if (gamma_value == 0) {
                    value = gammasRGB(color_value);
                } else {
                    value = set_gamma(color_value, gamma_value);
                }
                if (value >= 1 - 1 / 1e9)
                    value = 1;
                *outfile << (unsigned char) round(this->depth * value);
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

    void no_dithering(int bitness) {
        for (int i = 0; i < this->height; i++) {
            for (int j = 0; j < this->width; j++) {
                unsigned char color = findNearestPaletteColor(this->array[i][j], bitness);
                this->array[i][j] = color;
            }
        }
    }

    void ordered_dithering(int bitness) {
        double resizer = this->depth / (pow(2., (double) bitness) - 1);
//        this->depth = (pow(2., (double) bitness)-1);
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
                int tmp = (int) (this->array[i][j] + resizer * bayerMatrix[i % 8][j % 8]);
                if (tmp > 255) {
                    tmp = 255;
                }
                if (tmp < 0) {
                    tmp = 0;
                }
                unsigned char color = findNearestPaletteColor((unsigned char) tmp, bitness);
                this->array[i][j] = color;
            }
        }
    }

    void random_dithering(double gamma_value, int bitness) {
        double resizer = this->depth / (pow(2., (double) bitness) - 1);
        for (int i = 0; i < this->height; i++) {
            for (int j = 0; j < this->width; j++) {
                int tmp = (int) (this->array[i][j] + resizer * (-50 + rand() % 100) / 100);
                if (tmp > 255) {
                    tmp = 255;
                }
                if (tmp < 0) {
                    tmp = 0;
                }
                unsigned char color = findNearestPaletteColor((unsigned char) tmp, bitness);
                this->array[i][j] = color;
            }
        }
    }

    void Floyd_Steinberg_dithering(int bitness) {
        auto **err = new double *[(int) this->height];
        for (int i = 0; i < this->height; i++) {
            err[i] = new double[(int) this->width];
            for (int j = 0; j < this->width; j++) {
                err[i][j] = 0;
            }
        }
        for (int i = 0; i < this->height; i++)
            for (int j = 0; j < this->width; j++) {
                double tmp = (double) this->array[i][j] + err[i][j];
                if (tmp > 255)
                    tmp = 255;
                if (tmp < 0)
                    tmp = 0;
                this->array[i][j] = (unsigned char) tmp;
                unsigned char nc = findNearestPaletteColor(this->array[i][j], bitness);
                double error = (this->array[i][j] - nc) / 16.0;
                this->array[i][j] = nc;
                if (j + 1 < this->width)
                    err[i][j + 1] += error * 7;
                if (i + 1 < this->height) {
                    if (j - 1 >= 0)
                        err[i + 1][j - 1] += error * 3;
                    err[i + 1][j] += error * 5;
                    if (j + 1 < this->width)
                        err[i + 1][j + 1] += error;
                }
            }
        for (int i = 0; i < height; i++)
            delete[] err[i];
    }

    void jjn_dithering(int bitness) {
        auto **err = new double *[(int) this->height];
        for (int i = 0; i < this->height; i++) {
            err[i] = new double[(int) this->width];
            for (int j = 0; j < this->width; j++) {
                err[i][j] = 0;
            }
        }
        for (int i = 0; i < this->height; i++)
            for (int j = 0; j < this->width; j++) {
                double tmp = (double) this->array[i][j] + err[i][j];
                if (tmp > 255)
                    tmp = 255;
                if (tmp < 0)
                    tmp = 0;
                this->array[i][j] = (unsigned char) tmp;
                unsigned char nc = findNearestPaletteColor(this->array[i][j], bitness);
                double error = (this->array[i][j] - nc) / 48.0;
                this->array[i][j] = nc;
                if (j + 1 < this->width) err[i][j + 1] += error * 7;
                if (j + 2 < this->width) err[i][j + 2] += error * 5;
                if (i + 1 < this->height) {
                    if (j - 2 >= 0) err[i + 1][j - 2] += error * 3;
                    if (j - 1 >= 0) err[i + 1][j - 1] += error * 5;
                    err[i + 1][j] += error * 7;
                    if (j + 1 < this->width) err[i + 1][j + 1] += (error * 5);
                    if (j + 2 < this->width) err[i + 1][j + 2] += (error * 3);
                }
                if (i + 2 < this->height) {
                    if (j - 2 >= 0) err[i + 2][j - 2] += (error * 1);
                    if (j - 1 >= 0) err[i + 2][j - 1] += (error * 3);
                    err[i + 2][j] += (error * 5);
                    if (j + 1 < this->width) err[i + 2][j + 1] += (error * 3);
                    if (j + 2 < this->width) err[i + 2][j + 2] += (error * 1);
                }
            }
        for (int i = 0; i < height; i++)
            delete[] err[i];
    }

    void sierra_dithering(int bitness) {
        auto **err = new double *[(int) this->height];
        for (int i = 0; i < this->height; i++) {
            err[i] = new double[(int) this->width];
            for (int j = 0; j < this->width; j++) {
                err[i][j] = 0;
            }
        }
        for (int i = 0; i < this->height; i++)
            for (int j = 0; j < this->width; j++) {
                double tmp = (double) this->array[i][j] + err[i][j];
                if (tmp > 255)
                    tmp = 255;
                if (tmp < 0)
                    tmp = 0;
                this->array[i][j] = (unsigned char) tmp;
                unsigned char nc = findNearestPaletteColor(this->array[i][j], bitness);
                double error = (this->array[i][j] - nc) / 32.0;
                this->array[i][j] = nc;
                if (j + 1 < this->width) err[i][j + 1] += (error * 5);
                if (j + 2 < this->width) err[i][j + 2] += (error * 3);
                if (i + 1 < this->height) {
                    if (j - 2 >= 0) err[i + 1][j - 2] += (error * 2);
                    if (j - 1 >= 0) err[i + 1][j - 1] += (error * 4);
                    err[i + 1][j] += (error * 5);
                    if (j + 1 < this->width) err[i + 1][j + 1] += (error * 4);
                    if (j + 2 < this->width) err[i + 1][j + 2] += (error * 2);
                }
                if (i + 2 < this->height) {
                    if (j - 1 >= 0) err[i + 2][j - 1] += (error * 2);
                    err[i + 2][j] += (error * 3);
                    if (j + 1 < this->width) err[i + 2][j + 1] += (error * 2);
                }
            }
        for (int i = 0; i < height; i++)
            delete[] err[i];
    }

    void atikson_dithering(int bitness) {
        auto **err = new double *[(int) this->height];
        for (int i = 0; i < this->height; i++) {
            err[i] = new double[(int) this->width];
            for (int j = 0; j < this->width; j++) {
                err[i][j] = 0;
            }
        }
        for (int i = 0; i < this->height; i++)
            for (int j = 0; j < this->width; j++) {
                double tmp = (double) this->array[i][j] + err[i][j];
                if (tmp > 255)
                    tmp = 255;
                if (tmp < 0)
                    tmp = 0;
                this->array[i][j] = (unsigned char) tmp;
                unsigned char nc = findNearestPaletteColor(this->array[i][j], bitness);
                double error = (this->array[i][j] - nc) / 8.0;
                this->array[i][j] = nc;
                if (j + 1 < this->width) err[i][j + 1] += error;
                if (j + 2 < this->width) err[i][j + 2] += error;
                if (i + 1 < this->height) {
                    if (j - 1 >= 0) err[i + 1][j - 1] += error;
                    err[i + 1][j] += error;
                    if (j + 1 < this->width) err[i + 1][j + 1] += error;
                }
                if (i + 2 < this->height) {
                    err[i + 2][j] += error;
                }
            }
        for (int i = 0; i < height; i++)
            delete[] err[i];
    }

    void halftone_dithering(int bitness) {
        double halftoneMatrix[4][4] = {
                {7.,  13., 11., 4.},
                {12., 16., 14., 8.},
                {10., 15., 6.,  2.},
                {5.,  9.,  3.,  1.},
        };
        for (auto &i : halftoneMatrix) {
            for (double &j : i) {
                j = j / 16. - 0.5;
            }
        }
        double resizer = this->depth / (pow(2., (double) bitness) - 1);
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                int tmp = int((double) (this->array[i][j]) + resizer * halftoneMatrix[i % 4][j % 4]);
                if (tmp > 255) {
                    tmp = 255;
                }
                if (tmp < 0) {
                    tmp = 0;
                }
                this->array[i][j] = findNearestPaletteColor(tmp, bitness);
            }
    }

    explicit
    NetPBM(ifstream
           *file, double gamma, bool gradient) {
        this->
                file = file;
        this->

                read_header();

        file->ignore(1);

        this->
                array = new unsigned char *[height];
        for (
                int i = 0;
                i < this->
                        height;
                i++) {
            this->array[i] = new unsigned char[width];
        }
        if (!gradient)
            read_data(gamma);
        else {
            for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++) {
                    this->array[i][j] = (unsigned char) ((double) j * 256 / this->width);
                }
        }

    }

    ~

    NetPBM() {
        for (int i = 0; i < height; i++) {
            delete[] array[i];
        }
        delete[] array;
    }

};

#endif //LAB3_NETPBM_H

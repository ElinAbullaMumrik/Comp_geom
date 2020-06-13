//
// Created by yank0vy3rdna on 23.04.2020.
//
#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <set>

#ifndef LAB2KGIG_NETPBM_H
#define LAB2KGIG_NETPBM_H

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
//                array[i][j] = 0;
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

    void draw_point(int x, int y, double pixel_color, int brightness, double gamma_value) {
        double brightness_value = ((double) brightness) / (double) depth;

        if (!(x < width && y < height && x >= 0 && y >= 0 && brightness >= 0 && brightness <= depth))
            throw invalid_argument("Invalid argument");

        double value = ((double) this->array[y][x]) / this->depth;
        if (gamma_value == -1) {
            value = ungammasRGB(value);
            brightness_value = ungammasRGB(brightness_value);
        } else {
            value = ungamma(value, gamma_value);
            brightness_value = ungamma(brightness_value, gamma_value);
        }
        value = value + pixel_color * (brightness_value - value);
        if (gamma_value == -1) {
            value = gammasRGB(value);
        } else {
            value = gamma(value, gamma_value);
        }
        if (value >= 1 - 1 / 1e9)
            value = 1;
        int temp = round(this->depth * value);
        if (temp > 255)
            temp = 255;
        this->array[y][x] = (unsigned char) temp;
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

    double perp_point_y(double x1, double y1, double x2, double y2, double x) {
        return (-(x2 - x1) / (y2 - y1) * x + (y1 + y2) / 2 + (x2 - x1) / (y2 - y1) * (x1 + x2) / 2);
    }

    double perp_point_x(double x1, double y1, double x2, double y2, double y) {
        return (-(y2 - y1) / (x2 - x1) * y + (x1 + x2) / 2 + (y2 - y1) / (x2 - x1) * (y1 + y2) / 2);
    }

    double dist(double x1, double y1, double x2, double y2, double x, double y) {
        return abs((y2 - y1) * x - (x2 - x1) * y + x2 * y1 - y2 * x1) /
               (sqrt(pow((y2 - y1), 2) + pow((x2 - x1), 2)));
    }

    void draw_thick_line(point_t *point_1, point_t *point_2, double thickness, int brightness, double gamma_value) {
        double dy = abs(point_2->y - point_1->y);
        auto *rline_points = new set<pair<int, int>>;
        double dx = abs(point_2->x - point_1->x);
        if (dx != 0) {
            if (point_2->x < point_1->x) {
                swap(point_1, point_2);
            }
            double grad = 3 * thickness / dx;
            for (double i = point_1->x; i <= point_2->x;) {
                double j = (i - point_1->x) * (point_2->y - point_1->y) / (point_2->x - point_1->x) + point_1->y;
                for (int x = (int) (i - thickness / 2.) - 3;
                     x <= (int) (i + thickness / 2. + 1) + 3; x++) { // Find pixels in circle by searching in square
                    for (int y = (int) (j - thickness / 2.) - 3; y <= (int) (1 + j + thickness / 2.) + 3; y++) {
                        if (x >= 0 && y >= 0 && x < this->width && y < this->height)
                            rline_points->insert(make_pair(x, y));
                    }
                }
                i += grad;
            }
        } else {

            if (point_2->y < point_1->y) {
                swap(point_1, point_2);
            }
            double grad = 3 * thickness / dy;
            for (double i = point_1->y; i <= point_2->y;) {
                double j = (i - point_1->y) * (point_2->x - point_1->x) / (point_2->y - point_1->y) + point_1->x;
                for (int y = (int) (i - thickness / 2.) - 3;
                     y <= (int) (i + thickness / 2. + 1) + 3; y++) { // Find pixels in circle by searching in square
                    for (int x = (int) (j - thickness / 2.) - 3; x <= (int) (1 + j + thickness / 2.) + 5; x++) {
                        if (x >= 0 && y >= 0 && x < this->width && y < this->height)
                            rline_points->insert(make_pair(x, y));
                    }
                }
                i += grad;
            }
        }
        double koef;
        double y_perp_0, y_perp_1, x_perp_0, x_perp_1;
        if (dy != 0) {
            y_perp_0 = perp_point_y(point_1->x, point_1->y, point_2->x, point_2->y, 0);
            y_perp_1 = perp_point_y(point_1->x, point_1->y, point_2->x, point_2->y, 1);
        } else {
            x_perp_0 = perp_point_x(point_1->x, point_1->y, point_2->x, point_2->y, 0);
            x_perp_1 = perp_point_x(point_1->x, point_1->y, point_2->x, point_2->y, 1);

        }
        double line_length = (sqrt(pow((point_2->y - point_1->y), 2) + pow((point_2->x - point_1->x), 2)));

        for (pair<int, int> p : *rline_points) {
            int color = 0;
            int x = p.first;
            int y = p.second;
            for (int i = 0; i < 16; i++) {
                for (int j = 0; j < 16; j++) {
                    double x_choord = (double) x + ((double) i) / 16. + 1 / 32.;
                    double y_choord = (double) y + ((double) j) / 16. + 1 / 32.;
                    double s = dist(point_1->x, point_1->y, point_2->x, point_2->y, x_choord, y_choord);
                    double r;
                    if (dy != 0)
                        r = dist(0, y_perp_0, 1, y_perp_1, x_choord, y_choord);
                    else
                        r = dist(x_perp_1, 1, x_perp_0, 0, x_choord, y_choord);
                    if (s < thickness / 2. && r < line_length / 2.)
                        color++;

                }
            }
            if (color > 255)
                color = 255;
            draw_point(x, y, color / 255., brightness, gamma_value);
        }

//        }
        delete rline_points;
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


#endif //LAB2KGIG_NETPBM_H

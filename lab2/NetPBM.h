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

    void draw_point(int x, int y, double brightness, int color, double gamma_value) {
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
        auto * buf = new point_t;
        *buf = *a;
        *a = *b;
        *b = *buf;
        delete buf;
    }
    void draw_circle(point_t* point, double radius, int brightness, double gamma_value, double koef) {
        for (int x = (int) (point->x - radius) - 5;
             x <= (int) (point->x + radius + 1) + 5; x++) { // Find pixels in circle by searching in square
            for (int y = (int) (point->y - radius) - 5; y <= (int) (1 + point->y + radius) + 5; y++) {
                if(x >= 0 && y >= 0 && x < this->width && y < this->height) {
                    double r = sqrt(pow(x - point->x, 2) + pow(y - point->y, 2));
                    if (r <= radius - 0.5) {

                        draw_point(x, y, koef, brightness, gamma_value);
                    } else if (r <= radius + 1) {
                        draw_point(x, y, (-2. * r / (sqrt(2) + 2.) + 2. / (sqrt(2) + 2.) * (radius + 1.)) * koef,
                                   brightness,
                                   gamma_value);
                    }
                }
            }
        }
    }

public:
    int16_t getType() const {
        return type;
    }

    void draw_thick_line(point_t *point_1, point_t *point_2, double thickness, int brightness, double gamma_value) {
        double dy = abs(point_2->y - point_1->y);
        auto* rline_points = new set<pair<int, int>>;
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
                        if(x >= 0 && y >= 0 && x < this->width && y < this->height)
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
                        if(x >= 0 && y >= 0 && x < this->width && y < this->height)
                            rline_points->insert(make_pair(x, y));
                    }
                }
                i += grad;
            }
        }
        double koef;
        if (thickness < 1) {
            koef = thickness;
        } else {
            koef = 1;
        }
//        draw_circle(point_1, thickness/2., brightness, gamma_value, koef);
//        draw_circle(point_2, thickness/2., brightness, gamma_value, koef);
        for (pair<int, int> p : *rline_points) {
            int x = p.first;
            int y = p.second;
            double s = ((point_2->x - point_1->x)*((double )x - point_1->x)+(point_2->y - point_1->y)*((double )y - point_1->y))/sqrt(pow(point_1->x-point_2->x, 2)+pow(point_2->y-point_1->y, 2));
            if (s <= sqrt(pow(point_1->x-point_2->x, 2)+pow(point_2->y-point_1->y, 2))-0.5 && s >= 0) {
                double r = sqrt(pow(x * (point_2->y - point_1->y) - y * (point_2->x - point_1->x) +
                                    point_2->x * point_1->y - point_2->y * point_1->x, 2) /
                                (pow(point_2->y - point_1->y, 2) + pow(point_2->x - point_1->x, 2)));
                double alpha = atan2(abs(point_1->x-point_2->x),abs(point_1->y-point_2->y));

                if (r <= thickness / 2. - sqrt(2)/2.*sin(3.14/4.+alpha)) {
                    draw_point(x, y, koef, brightness, gamma_value);
                } else if (r <= thickness / 2.+sqrt(2)/2.*sin(3.14/4.+alpha)) { // abs(thickness / 2. - sqrt(2) / 2. + r * (-0.5 - sqrt(2) / 2.))
                    draw_point(x, y, (-2. * r / (sqrt(2) + 2.) + 2. / (sqrt(2) + 2.) * (thickness / 2. + 1.)) * koef,
                               brightness,
                               gamma_value);
                }
            }
        }
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

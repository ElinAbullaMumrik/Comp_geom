//
// Created by elin on 14.06.20.
//

#ifndef LAB4_PPM_H
#define LAB4_PPM_H

#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstdlib>
#include <cmath>

using namespace std;
struct pixel_t {
    unsigned char R;
    unsigned char G;
    unsigned char B;

};

class PPM {
private:
    ifstream *file;
    uint16_t type = -1;
    uint16_t width = -1;
    uint16_t height = -1;
    uint16_t depth = -1;
    pixel_t **array;

    void read_header() {
        unsigned char buf;
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
                unsigned char tmp;
                *this->file >> tmp;
                this->array[i][j].R = tmp;
                *this->file >> tmp;
                this->array[i][j].G = tmp;
                *this->file >> tmp;
                this->array[i][j].B = tmp;
            }
        }
    }

    void write_to_file(ofstream *outfile, double gamma_value) {
        *outfile << "P5" << (unsigned char) (10) << width << " " << height << (unsigned char) (10) << depth
                 << (unsigned char) (10);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                *outfile << array[i][j].R;
                *outfile << array[i][j].G;
                *outfile << array[i][j].B;
            }
        }
    }

    explicit
    PPM(ifstream *file) {
        this->file = file;
        this->read_header();
        file->ignore(1);
        this->array = new pixel_t *[height];
        for (int i = 0; i < this->height; i++) {
            this->array[i] = new pixel_t[width];
        }
        read_data();
    }

    ~PPM() {
        for (int i = 0; i < height; i++) {
            delete[] array[i];
        }
        delete[] array;
    }
};

#endif //LAB4_PPM_H

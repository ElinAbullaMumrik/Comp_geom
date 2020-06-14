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
#include "NetPBM.h"

using namespace std;
struct pixel_t {
    unsigned char R;
    unsigned char G;
    unsigned char B;

};

class PPM : public NetPBM {
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

    void write_to_file(ofstream *outfile) override {
        *outfile << "P6" << (unsigned char) (10) << width << " " << height << (unsigned char) (10) << depth
                 << (unsigned char) (10);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                *outfile << array[i][j].R;
                *outfile << array[i][j].G;
                *outfile << array[i][j].B;
            }
        }
    }

public:
    void inversion() override {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                array[i][j].R = 255 - array[i][j].R;
                array[i][j].G = 255 - array[i][j].G;
                array[i][j].B = 255 - array[i][j].B;
            }
        }
    }

    void horizontal_mirror() override {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < (width - 1) / 2; j++) {
                pixel_t temp = array[i][j];
                array[i][j] = array[i][width - j - 1];
                array[i][width - j - 1] = temp;
            }
        }
    }

    void rotate_90(bool isclock) override {
        int tmp = this->width;
        this->width = this->height;
        this->height = tmp;
        pixel_t **arr;
        arr = new pixel_t *[height];
        for (int i = 0; i < this->height; i++) {
            arr[i] = new pixel_t[width];
            for (int j = 0; j < width; j++) {
                if (!isclock)
                    arr[i][j] = this->array[j][height - i - 1];
                else
                    arr[i][j] = this->array[width - 1 - j][i];
            }
        }
        for (int i = 0; i < width; i++) {
            delete[] array[i];
        }
        delete[] array;

        array = arr;
    };

    void vertical_mirror() override {
        for (int i = 0; i < (height - 1) / 2; i++) {
            for (int j = 0; j < width; j++) {
                int R, G, B;
                pixel_t temp = array[i][j];
                array[i][j] = array[height - i - 1][j];
                array[height - i - 1][j] = temp;
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

    ~PPM() override {
        for (int i = 0; i < height; i++) {
            delete[] array[i];
        }
        delete[] array;
    }
};

#endif //LAB4_PPM_H

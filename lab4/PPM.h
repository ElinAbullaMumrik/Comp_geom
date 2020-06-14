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
#include "pixel_t.h"
#include "Converter.h"

using namespace std;

class PPM{
private:
    ifstream *file;
    ifstream *file1;
    ifstream *file2;
    ifstream *file3;
    int count;
    uint16_t type = -1;
    uint16_t width = -1;
    uint16_t height = -1;
    uint16_t depth = -1;
    pixel_t **array;

    void read_header() {
        if(count == 1) {
            unsigned char buf;
            *this->file >> buf;
            if (buf != 'P') {
                throw logic_error("Bad file");
            }
            *this->file >> buf;
            if (buf == '1' || buf == '2' || buf == '3' || buf == '4' || buf == '5' || buf == '7') {
                throw logic_error("Not supported type of NetPCM");
            } else if (buf == '6')
                this->type = 6;
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
        }else if (count==3){
            unsigned char buf;
            *this->file1 >> buf;
            if (buf != 'P') {
                throw logic_error("Bad file");
            }
            *this->file1 >> buf;
            if (buf == '1' || buf == '2' || buf == '3' || buf == '4' || buf == '6' || buf == '7') {
                throw logic_error("Not supported type of NetPCM");
            } else if (buf == '5')
                this->type = 5;
            else {
                throw logic_error("Bad file");
            }
            *this->file1 >> buf;
            *this->file1 >> this->width;
            *this->file1 >> buf;
            *this->file1 >> this->height;
            *this->file1 >> buf;
            *this->file1 >> this->depth;
            if (this->depth != 255) {
                throw logic_error("Not supported non 255 colors count");
            }


            *this->file2 >> buf;
            if (buf != 'P') {
                throw logic_error("Bad file");
            }
            *this->file2 >> buf;
            if (buf == '1' || buf == '2' || buf == '3' || buf == '4' || buf == '6' || buf == '7') {
                throw logic_error("Not supported type of NetPCM");
            } else if (buf == '5')
                this->type = 5;
            else {
                throw logic_error("Bad file");
            }
            *this->file2 >> buf;
            *this->file2 >> this->width;
            *this->file2 >> buf;
            *this->file2 >> this->height;
            *this->file2 >> buf;
            *this->file2 >> this->depth;
            if (this->depth != 255) {
                throw logic_error("Not supported non 255 colors count");
            }

            *this->file3 >> buf;
            if (buf != 'P') {
                throw logic_error("Bad file");
            }
            *this->file3 >> buf;
            if (buf == '1' || buf == '2' || buf == '3' || buf == '4' || buf == '6' || buf == '7') {
                throw logic_error("Not supported type of NetPCM");
            } else if (buf == '5')
                this->type = 5;
            else {
                throw logic_error("Bad file");
            }
            *this->file3 >> buf;
            *this->file3 >> this->width;
            *this->file3 >> buf;
            *this->file3 >> this->height;
            *this->file3 >> buf;
            *this->file3 >> this->depth;
            if (this->depth != 255) {
                throw logic_error("Not supported non 255 colors count");
            }
        }
    }

    void read_data() {
        if(count == 1) {
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
        } else if (count==3){
            for (int i = 0; i < this->height; i++) {
                for (int j = 0; j < this->width; j++) {
                    unsigned char tmp;
                    *this->file1 >> tmp;
                    this->array[i][j].R = tmp;
                    *this->file2 >> tmp;
                    this->array[i][j].G = tmp;
                    *this->file3 >> tmp;
                    this->array[i][j].B = tmp;
                }
            }
        }
    }

public:
    void write_to_file(ofstream *outfile) {
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
    void write_to_file(ofstream *outfile1,ofstream *outfile2,ofstream *outfile3) {
        *outfile1 << "P5" << (unsigned char) (10) << width << " " << height << (unsigned char) (10) << depth
                  << (unsigned char) (10);
        *outfile2 << "P5" << (unsigned char) (10) << width << " " << height << (unsigned char) (10) << depth
                  << (unsigned char) (10);
        *outfile3 << "P5" << (unsigned char) (10) << width << " " << height << (unsigned char) (10) << depth
                  << (unsigned char) (10);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                *outfile1 << array[i][j].R;
                *outfile2 << array[i][j].G;
                *outfile3 << array[i][j].B;
            }
        }
    }
    void makeConvertion(Converter* converter){
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                array[i][j] = converter->convert(array[i][j]);
            }
        }
    }
    explicit
    PPM(ifstream *file) {
        this->count = 1;
        this->file = file;
        this->read_header();
        file->ignore(1);
        this->array = new pixel_t *[height];
        for (int i = 0; i < this->height; i++) {
            this->array[i] = new pixel_t[width];
        }
        read_data();
    }
    PPM(ifstream *file1,ifstream *file2,ifstream *file3) {
        this->count=3;
        this->file1 = file1;
        this->file2 = file2;
        this->file3 = file3;
        this->read_header();
        file1->ignore(1);
        file2->ignore(1);
        file3->ignore(1);
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

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

class NetPBM {
public:
//    virtual int16_t getType() const;
    virtual void write_to_file(ofstream *outfile) = 0;

    virtual void inversion() = 0;

    virtual void horizontal_mirror() = 0;

    virtual void vertical_mirror() = 0;

    virtual void rotate_90(bool isclock) = 0;

    virtual ~NetPBM() = default;

};

#endif //LAB3_NETPBM_H

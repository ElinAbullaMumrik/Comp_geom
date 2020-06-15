//
// Created by elin on 14.06.20.
//

#include <string>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <set>
#include "PPM.h"
#include "converters/BaseConverter.h"
#include "converters/fromRGBtoHSL.h"
#include "converters/fromRGBtoHSV.h"
#include "converters/fromRGBtoYCbCr601.h"
#include "converters/fromRGBtoYCbCr709.h"
#include "converters/fromRGBtoCMY.h"
#include "converters/fromRGBtoYCoCg.h"

#include "converters/fromCMYtoRGB.h"
#include "converters/fromHSLtoRGB.h"
#include "converters/fromHSVtoRGB.h"
#include "converters/fromYCbCr601toRGB.h"
#include "converters/fromYCbCr709toRGB.h"
#include "converters/fromYCoCgtoRGB.h"


char **getCmdOption(char **begin, char **end, const std::string &option, int count_options) {
    char **itr = std::find(begin, end, option);
    char **answer = new char *[count_options];
    bool flag = false;
    for (int i = 0; i < count_options; i++) {
        if (itr != end) {
            itr++;
            if (strcmp(*itr, "-f") == 0 || strcmp(*itr, "-t") == 0 || strcmp(*itr, "-i") == 0 ||
                strcmp(*itr, "-o") == 0) {
                flag = true;
                break;
            }
            answer[i] = *itr;
        } else {
            flag = true;
            break;
        }
    }
    if (flag) {
        delete[]answer;
        return nullptr;
    }

    return answer;
}

bool check_if_color_space(char *color_space) {
    std::set<char *> st;
    st.insert("RGB");
    st.insert("HSL");
    st.insert("HSV");
    st.insert("YCbCr.601");
    st.insert("YCbCr.709");
    st.insert("YCoCg");
    st.insert("CMY");
    for (const char *i : st) {
        if ((int) strcmp(color_space, i) == 0)
            return true;
    }
    return false;
}

char **parse_format(char *inp) {
    int lastdot = -1;
    int count_symbols = 0;
    for (int i = 0; inp[i] != 0; i++) {
        if (inp[i] == '.') {
            lastdot = i;
        }
        count_symbols++;
    }
    if (lastdot == -1)
        return nullptr;
    char **answer = new char *[3];
    for (int l = 0; l < 3; l++) {
        answer[l] = new char[count_symbols + 3];
        for (int i = 0; i < lastdot; i++) {
            answer[l][i] = inp[i];
        }
        answer[l][lastdot] = '_';
        answer[l][lastdot + 1] = '1' + l;
        for (int i = lastdot; i < count_symbols; i++) {
            answer[l][i + 2] = inp[i];
        }
        answer[l][count_symbols + 2] = 0;
    }
    return answer;
}

Converter *getConverterByName(bool isfrom, char *name) {

    if (strcmp("RGB", name) == 0)
        return new BaseConverter();
    if (isfrom) { //from...toRGB
        if (strcmp("HSL", name) == 0)
            return new fromHSLtoRGB();
        if (strcmp("HSV", name) == 0)
            return new fromHSVtoRGB();
        if (strcmp("YCbCr.601", name) == 0)
            return new fromYCbCr601toRGB();
        if (strcmp("YCbCr.709", name) == 0)
            return new fromYCbCr709toRGB();
        if (strcmp("YCoCg", name) == 0)
            return new fromYCoCgtoRGB();
        if (strcmp("CMY", name) == 0)
            return new fromCMYtoRGB();
    } else { //fromRGBto...
        if (strcmp("HSL", name) == 0)
            return new fromRGBtoHSL();
        if (strcmp("HSV", name) == 0)
            return new fromRGBtoHSV();
        if (strcmp("YCbCr.601", name) == 0)
            return new fromRGBtoYCbCr601();
        if (strcmp("YCbCr.709", name) == 0)
            return new fromRGBtoYCbCr709();
        if (strcmp("YCoCg", name) == 0)
            return new fromRGBtoYCoCg();
        if (strcmp("CMY", name) == 0)
            return new fromRGBtoCMY();
    }
    return nullptr;
}

int main(int argc, char **argv) {
    try {
        char **to_space_name = getCmdOption(argv, argv + argc, "-t", 1);
        if (to_space_name == nullptr) {
            throw std::invalid_argument("Кривая опция -t");
        }
        if (!check_if_color_space(to_space_name[0])) {
            delete[]to_space_name;
            throw std::invalid_argument("Ты написал невалидный colorspace");
        }
        char **from_color_space = getCmdOption(argv, argv + argc, "-f", 1);
        if (from_color_space == nullptr) {
            delete[]to_space_name;
            throw std::invalid_argument("Кривая опция -f");
        }
        if (!check_if_color_space(from_color_space[0])) {
            delete[]to_space_name;
            delete[]from_color_space;
            throw std::invalid_argument("Ты написал невалидный colorspace");
        }
        char **input_file_name = getCmdOption(argv, argv + argc, "-i", 2);
        if (input_file_name == nullptr) {
            delete[]to_space_name;
            delete[]from_color_space;
            throw std::invalid_argument("Кривая опция -i");
        }
        char in_count = input_file_name[0][0];
        char **inputfilenames;
        if (in_count == '1') {
            inputfilenames = new char *[1];
            inputfilenames[0] = input_file_name[1];
        } else if (in_count == '3') {
            inputfilenames = parse_format(input_file_name[1]);
            if (inputfilenames == nullptr) {
                delete[]to_space_name;
                delete[]from_color_space;
                delete[]input_file_name;
                throw std::invalid_argument("В пути к файлу нет расширения");
            }
        } else {
            delete[]to_space_name;
            delete[]from_color_space;
            delete[]input_file_name;
            throw std::invalid_argument("Кривой count");
        }
        delete[] input_file_name;
        input_file_name = inputfilenames;


        char **output_file_name = getCmdOption(argv, argv + argc, "-o", 2);
        if (output_file_name == nullptr) {
            delete[]to_space_name;
            delete[]from_color_space;
            delete[]input_file_name;
            throw std::invalid_argument("Кривая опция -o");
        }
        char out_count = output_file_name[0][0];
        char **outputfilenames;
        if (out_count == '1') {
            outputfilenames = new char *[1];
            outputfilenames[0] = output_file_name[1];
        } else if (out_count == '3') {
            outputfilenames = parse_format(output_file_name[1]);
            if (outputfilenames == nullptr) {
                delete[]to_space_name;
                delete[]from_color_space;
                delete[]output_file_name;
                throw std::invalid_argument("В пути к файлу нет расширения");
            }
        } else {
            delete[]to_space_name;
            delete[]from_color_space;
            delete[]output_file_name;
            throw std::invalid_argument("Кривой count");
        }
        delete[] output_file_name;
        output_file_name = outputfilenames;

        PPM *picture = nullptr;
        ifstream file1;
        file1.unsetf(ios_base::skipws);
        ifstream file2;
        file2.unsetf(ios_base::skipws);
        ifstream file3;
        file3.unsetf(ios_base::skipws);
        if (in_count == '1') {
            file1.open(input_file_name[0], std::ios::binary);
            if (!file1.is_open()) {
                delete[]to_space_name;
                delete[]from_color_space;
                delete[]input_file_name;
                delete[]output_file_name;
                delete picture;
                if (file1.is_open())
                    file1.close();
                throw std::invalid_argument("Cannot open input file");
            }
            picture = new PPM(&file1);
        } else {
            file1.open(input_file_name[0], std::ios::binary);
            file2.open(input_file_name[1], std::ios::binary);
            file3.open(input_file_name[2], std::ios::binary);
            if (!file1.is_open() || !file2.is_open() || !file3.is_open()) {
                delete[]to_space_name;
                delete[]from_color_space;
                delete[]input_file_name;
                delete[]output_file_name;
                delete picture;
                if (file1.is_open())
                    file1.close();
                if (file2.is_open())
                    file2.close();
                if (file3.is_open())
                    file3.close();
                throw std::invalid_argument("Cannot open input file");
            }
            picture = new PPM(&file1, &file2, &file3);
        }


        Converter *to = getConverterByName(false, to_space_name[0]);
        Converter *from = getConverterByName(true, from_color_space[0]);
        picture->makeConvertion(from);
        picture->makeConvertion(to);


        ofstream outfile1;
        ofstream outfile2;
        ofstream outfile3;

        if (out_count == '1') {
            outfile1.open(output_file_name[0], std::ios::binary);
            if (!outfile1.is_open()) {
                delete[]to_space_name;
                delete[]from_color_space;
                delete[]input_file_name;
                delete[]output_file_name;
                delete picture;
                if (file1.is_open())
                    file1.close();
                if (file2.is_open())
                    file2.close();
                if (file3.is_open())
                    file3.close();
                if (outfile1.is_open())
                    outfile1.close();
                throw std::invalid_argument("Cannot open output file");
            }
            picture->write_to_file(&outfile1);
        } else {
            outfile1.open(output_file_name[0], std::ios::binary);
            outfile2.open(output_file_name[1], std::ios::binary);
            outfile3.open(output_file_name[2], std::ios::binary);
            if (!outfile1.is_open() || !outfile2.is_open() || !outfile3.is_open()) {
                delete[]to_space_name;
                delete[]from_color_space;
                delete[]input_file_name;
                delete[]output_file_name;
                delete picture;
                if (file1.is_open())
                    file1.close();
                if (file2.is_open())
                    file2.close();
                if (file3.is_open())
                    file3.close();
                if (outfile1.is_open())
                    outfile1.close();
                if (outfile2.is_open())
                    outfile2.close();
                if (outfile3.is_open())
                    outfile3.close();
                throw std::invalid_argument("Cannot open output file");
            }
            picture->write_to_file(&outfile1, &outfile2, &outfile3);
        }

        delete[]to_space_name;
        delete[]from_color_space;
        delete[]input_file_name;
        delete[]output_file_name;
        delete picture;
        if (file1.is_open())
            file1.close();
        if (file2.is_open())
            file2.close();
        if (file3.is_open())
            file3.close();
        if (outfile1.is_open())
            outfile1.close();
        if (outfile2.is_open())
            outfile2.close();
        if (outfile3.is_open())
            outfile3.close();
        return 0;

    } catch (const std::invalid_argument &error) {
        std::cerr << error.what() << std::endl;
        return 1;
    }
}
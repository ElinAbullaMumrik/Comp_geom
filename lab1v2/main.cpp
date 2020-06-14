#include <iostream>
#include <cstdlib>
#include <set>
#include "PPM.h"
#include "PBM.h"
#include "NetPBM.h"

using namespace std;

int main(int argc, char **argv) {
    try {
        if (argc == 666) {
            throw logic_error("Invalid user");
        }
        if (argc != 4) {
            throw logic_error("Invalid argument count");
        }
        char *infilename = argv[1];
        char *outfilename = argv[2];
        int conversion = atoi(argv[3]);
        auto *file = new ifstream(infilename, std::ios::binary);
        (*file).unsetf(ios_base::skipws);
        if (!(*file).is_open()) {
            delete file;
            throw logic_error("Can't open file to read");
        }
        unsigned char buf;
        *file >> buf;
        if (buf != 'P') {
            throw logic_error("Bad file");
        }
        *file >> buf;
        if (buf == '1' || buf == '2' || buf == '3' || buf == '4' || buf == '7') {
            throw logic_error("Not supported type of NetPCM");
        } else if (buf != '5'&& buf != '6'){
            throw logic_error("Bad file");
        }
        NetPBM* picture;
        if (buf == '5') {
            picture = new PBM(file);
        }
        if (buf == '6'){
            picture = new PPM(file);
        }
        switch (conversion) {
            case 0:
                picture->inversion();
                break;
            case 1:
                picture->horizontal_mirror();
                break;
            case 2:
                picture->vertical_mirror();
                break;
            case 3:
                picture->rotate_90(true);
                break;
            case 4:
                picture->rotate_90(false);
                break;
        }


        auto *outfile = new ofstream(outfilename);
        if (!outfile->is_open()) {
            file->close();
            delete file;
            delete outfile;
            delete picture;
            throw logic_error("Can't open file to write");
        }

        picture->write_to_file(outfile);
        outfile->close();
        file->close();
        delete file;
        delete outfile;
        delete picture;
        return 0;
    }
    catch (const logic_error &error) {
        cerr << error.what() << endl;
        return 1;
    }
}
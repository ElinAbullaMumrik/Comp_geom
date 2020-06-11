#include <iostream>
#include <cstdlib>
#include <set>
#include "NetPBM.h"

using namespace std;

int main(int argc, char **argv) {
    try {
        if (argc == 666) {
            throw logic_error("Invalid user");
        }
        if (argc != 7) {
            throw logic_error("Invalid argument count");
        }
        char *infilename = argv[1];
        char *outfilename = argv[2];
        bool gradient = atoi(argv[3]);
        int dithering = atoi(argv[4]);
        int bitness = atoi(argv[5]);
        double gamma = atof(argv[6]);
        auto *file = new ifstream(infilename, std::ios::binary);
        (*file).unsetf(ios_base::skipws);
        if (!(*file).is_open()) {
            delete file;
            throw logic_error("Can't open file to read");
        }

        auto *netPcm = new NetPBM(file, gamma, gradient);
        switch (dithering) {
            case 0:
                netPcm->no_dithering(bitness);
                break;

            case 1:
                netPcm->ordered_dithering(bitness);
                break;

            case 2:
                netPcm->random_dithering(gamma, bitness);
                break;

            case 3:
                netPcm->Floyd_Steinberg_dithering(bitness);
                break;
            case 4:
                netPcm->jjn_dithering(bitness);
                break;
            case 5:
                netPcm->sierra_dithering(bitness);
                break;
            case 6:
                netPcm->atikson_dithering(bitness);
                break;
            case 7:
                netPcm->halftone_dithering(bitness);
                break;

        }

        auto *outfile = new ofstream(outfilename, std::ios::binary);
        if (!outfile->is_open()) {
            file->close();
            delete file;
            delete outfile;
            delete netPcm;
            throw logic_error("Can't open file to write");
        }

        netPcm->write_to_file(outfile, gamma);
        outfile->close();
        file->close();
        delete file;
        delete outfile;
        delete netPcm;
        return 0;
    }
    catch (const logic_error &error) {
        cerr << error.what() << endl;
        return 1;
    }
}
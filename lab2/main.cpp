

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
        if (argc != 9 && argc != 10) {
            throw logic_error("Invalid argument count");
        }
        char *infilename = argv[1];
        char *outfilename = argv[2];
        int brightness = atoi(argv[3]);
        double line_width = atof(argv[4]);
        auto *point_1 = new point_t();
        (*point_1).x = atof(argv[5]);
        (*point_1).y = atof(argv[6]);
        auto *point_2 = new point_t();
        (*point_2).x = atof(argv[7]);
        (*point_2).y = atof(argv[8]);
        double gamma = -1;
        if (argc == 10) {
            gamma = atof(argv[9]);
            if (gamma <= 0) {
                throw logic_error("Invalid gamma");
            }
        }
        auto * file = new ifstream (infilename, std::ios::binary);
        (*file).unsetf(ios_base::skipws);
        if (!(*file).is_open()) {
            delete file;
            delete point_1;
            delete point_2;
            throw logic_error("Can't open file to read");
        }
        auto *netPcm = new NetPBM(file);
        netPcm->draw_thick_line(point_1, point_2, line_width, brightness, gamma);
        auto* outfile = new ofstream (outfilename, std::ios::binary);
        if (!outfile->is_open()) {
            file->close();
            delete file;
            delete outfile;
            delete netPcm;
            delete point_1;
            delete point_2;
            throw logic_error("Can't open file to write");
        }
        netPcm->write_to_file(outfile);
        outfile->close();
        file->close();
        delete file;
        delete outfile;
        delete netPcm;
        delete point_1;
        delete point_2;
        return 0;
    }
    catch (const logic_error &error) {
        cerr << error.what() << endl;
        return 1;
    }
}

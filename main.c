#include <stdio.h>
#include <stdlib.h>
#include "work_with_PNM.h"

int main(int argc, char *argv[]) {
    char *filename = argv[1];
    char *outname = argv[2];
    header_t *header = (header_t *) malloc(sizeof(header_t));
    FILE *input = fopen(filename, "rb");

    if (!input) {
        printf("Error: file not found\n");
        return 2;
    }
    FILE *outptr = fopen(outname, "wb");

    get_header(input, header);
    if (header->type_pnm2 == 5) {
        unsigned char *picture = (unsigned char *) malloc(sizeof(char) * header->height * header->width);
        if (!picture) {
            printf("Error: troubles with memory\n");
            return 2;
        }
        from_pnm_gray(input, picture, *header);
        switch (*argv[3]) {
            case '0':
                inverse_gray(picture, header);
                break;
            case '1':
                mirror_horizontal_gray(picture, header);
                break;
            case '2':
                mirror_vertical_gray(picture, header);
                break;
            case '3':
                rotate_left_90_gray(picture, header);
                break;
            case '4':
                rotate_right_90_gray(picture, header);
                break;
            default:
                printf("wrong command");
        }
        to_pnm_gray(outptr, picture, *header);
        free(picture);
    } else if (header->type_pnm2 == 6) {
        pixel_t *picture = (pixel_t *) malloc(sizeof(pixel_t) * header->height * header->width);
        if (!picture) {
            printf("Error: troubles with memory\n");
            return 2;
        }
        from_pnm_color(input, picture, *header);
        switch (*argv[3]) {
            case '0':
                inverse_color(picture, header);
                break;
            case '1':
                mirror_horizontal(picture, header);
                break;
            case '2':
                mirror_vertical(picture, header);
                break;
            case '3':
                rotate_left_90(picture, header);
                break;
            case '4':
                rotate_right_90(picture, header);
                break;
            default:
                printf("wrong command");

        }
        to_pnm_color(outptr, picture, *header);
        free(picture);
    } else {
        printf("Error: unknown format\n");
        return 2;
    }

    fclose(outptr);
    return 0;
}

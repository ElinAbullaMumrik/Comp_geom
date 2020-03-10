#include <stdio.h>
#include <stdlib.h>
#include "work_with_PNM.h"

long int filesize( FILE *fp )
{
    long int save_pos, size_of_file;

    save_pos = ftell( fp );
    fseek( fp, 0L, SEEK_END );
    size_of_file = ftell( fp );
    fseek( fp, save_pos, SEEK_SET );
    return( size_of_file );
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Error: not enough arguments\n");
        return 2;
    }
    if ((*argv[3] != '0') && (*argv[3] != '1') && (*argv[3] != '2') && (*argv[3] != '3') && (*argv[3] != '4')) {
        printf("Error: wrong arguments\n");
        return 2;
    }
    char *filename = argv[1];

    char *outname = argv[2];
    header_t *header = (header_t *) malloc(sizeof(header_t));


    FILE *input = fopen(filename, "rb");
    long lengthfile = filesize(input);

    if (!input) {
        printf("Error: file not found\n");
        return 2;
    }

    get_header(input, header);

    if (header->type_pnm2 == 5 && header->type_pnm == 'P') {
        int x = header->width*header->height+ sizeof(header);
        if (x>lengthfile){
            printf("Error: wrong size of file\n");
            return 2;
        }
        unsigned char *picture = (unsigned char *) malloc(sizeof(unsigned char) * header->height * header->width);
        if (!picture) {
            printf("Error: troubles with memory\n");
            free(header);
            fclose(input);
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
            case '4':
                rotate_left_90_gray(picture, header);
                break;
            case '3':
                rotate_right_90_gray(picture, header);
                break;
            default:
                printf("wrong command");
        }
        FILE *outptr = fopen(outname, "wb");
        if (!outptr) {
            printf("Error: wrong output file\n");
            return 2;
        }
        to_pnm_gray(outptr, picture, *header);
        fclose(outptr);
        free(picture);
    } else if (header->type_pnm2 == 6 && header->type_pnm == 'P') {
        int x = 3*header->width*header->height+ sizeof(header);
        if (x>lengthfile){
            printf("Error: wrong size of file\n");
            return 2;
        }
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
            case '4':
                rotate_left_90(picture, header);
                break;
            case '3':
                rotate_right_90(picture, header);
                break;
            default:
                printf("wrong command");
        }
        FILE *outptr = fopen(outname, "wb");
        if (!outptr) {
            printf("Error: wrong output file\n");
            return 2;
        }
        to_pnm_color(outptr, picture, *header);
        fclose(outptr);
        free(picture);
    } else {
        printf("Error: unknown format\n");
        return 2;
    }
    free(header);
    fclose(input);
    return 0;
}

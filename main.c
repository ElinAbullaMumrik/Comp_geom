#include <stdio.h>
#include <stdlib.h>
#include "work_with_PNM.h"

int main() {
    char const *filename = "s.pnm";

    header_t *header = (header_t *) malloc(sizeof(header_t));
    FILE *input = fopen(filename, "rb");
    FILE *outptr = fopen("r.pnm", "wb");

    get_header(input, header);
    pnm_picture_t *picture = (pnm_picture_t *) malloc(sizeof(pixel_t) * header->height * header->width + sizeof(header));
    picture->h.width = header->width;
    picture->h.height = header->height;
    picture->h.deepth = header->deepth;
    picture->h.type_pnm  = header->type_pnm;
    picture->h.type_pnm2 = header->type_pnm2;
    fread(picture->data, sizeof(pixel_t), picture->h.width * picture->h.height, input);
    printf("%s", picture->data);
//    from_pnm(input, picture);
//    printf("%s", picture->data);

    to_pnm(outptr, &picture);
//    pnm_picture_t ready_picture = rotate_left_90 (*picture);
//    pixel_t* my_picture = (pixel_t*)malloc(height * width * sizeof(pixel_t));
//    fread(my_picture, 1, width * height * sizeof(pixel_t), input);
    fclose(input);
    return 0;
}

//
// Created by Людмила on 12.02.2020.
//
#include <stdio.h>
#include "work_with_PNM.h"
#include <stdlib.h>

void get_header(FILE *input, header_t *head) {
    fscanf(input, "%c%i%i%i%i", &head->type_pnm, &head->type_pnm2, &head->width, &head->height, &head->deepth);
    printf("%c%i%i%i%i", head->type_pnm, head->type_pnm2, head->width, head->height, head->deepth);
//    return READ_OK;
}

void from_pnm_color(FILE *input, pixel_t *read, header_t header) {
    fread(read, 1, header.width * header.height * sizeof(struct pixel_t), input);//    return READ_OK;
}


void to_pnm(FILE *output, pixel_t *write, header_t write_header) {
    fprintf(output, "%c%i\n%i %i\n%i",
            write_header.type_pnm, write_header.type_pnm2, write_header.width, write_header.height,
            write_header.deepth);
    fwrite(write, 1, write_header.width * write_header.height * sizeof(pixel_t), output);
}

pixel_t *rotate_left_90(pixel_t *image, header_t header) {
    pnm_picture_t *rotated = malloc(sizeof(header.height * header.width) + sizeof(header));
    rotated->h.width = header.height;
    rotated->h.height = header.width;
    pixel_t *rotate = malloc(abs(rotated->h.width) * rotated->h.height * sizeof(pixel_t));

    for (int i = 0; i < abs(header.height); ++i) {
        for (int j = 0; j < header.width; ++j) {
            pixel_t temp = image[header.width * i + j];
            rotate[abs(rotated->h.width) * (j + 1) - i - 1] = temp;
        }
    }
    rotated->data = rotate;
    return rotated;
}
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

void from_pnm(FILE *input, pnm_picture_t *read) {
    fread(read->data, 1, read->h.width * read->h.height * sizeof(pixel_t), input);
//    return READ_OK;
}


void to_pnm(FILE *output, pnm_picture_t *write) {
    fprintf(output, "%c%i%i%i%i", write->h.type_pnm, write->h.type_pnm2, write->h.width, write->h.height, write->h.deepth);
    fwrite(write->data, 1, write->h.width * write->h.height * sizeof(pixel_t), write);
}

pnm_picture_t rotate_left_90(const pnm_picture_t image) {
    pnm_picture_t *rotated = malloc(sizeof(image.h.height*image.h.width)+ sizeof(image.h));
    rotated->h.width = image.h.height;
    rotated->h.height = image.h.width;
    pixel_t *rotate = malloc(abs(rotated->h.width) * rotated->h.height * sizeof(pixel_t));

    for (int i = 0; i < abs(image.h.height); ++i) {
        for (int j = 0; j < image.h.width; ++j) {
            pixel_t temp = image.data[image.h.width * i + j];
            rotate[abs(rotated->h.width) * (j + 1) - i - 1] = temp;
        }
    }
    rotated->data = rotate;
    return * rotated;
}
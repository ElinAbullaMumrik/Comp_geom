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

void from_pnm_gray(FILE *input, pixel_t *read, header_t header) {
    fread(read, 1, header.width * header.height * sizeof(char), input);//    return READ_OK;
}

void to_pnm_color(FILE *output, pixel_t *write, header_t write_header) {
    fprintf(output, "%c%i\n%i %i\n%i\n",
            write_header.type_pnm, write_header.type_pnm2, write_header.width, write_header.height,
            write_header.deepth);
    fwrite(write, 1, write_header.width * write_header.height * sizeof(pixel_t), output);
}

void to_pnm_gray(FILE *output, pixel_t *write, header_t write_header) {
    fprintf(output, "%c%i\n%i %i\n%i\n",
            write_header.type_pnm, write_header.type_pnm2, write_header.width, write_header.height,
            write_header.deepth);
    fwrite(write, 1, write_header.width * write_header.height * sizeof(char), output);
}

void inverse_color(pixel_t *image, header_t *header) {
//    pixel_t *inversed = malloc((header->width) * header->height * sizeof(pixel_t));
    for (int i = 0; i < header->height*header->width; i++) {
            pixel_t temp = image[i ];
            temp.b = 255 - temp.b;
            temp.g = 255 - temp.g;
            temp.r = 255 - temp.r;
            image[i] = temp;
        }
}

void inverse_gray(unsigned char *image, header_t *header) {
//    pixel_t *inversed = malloc((header->width) * header->height * sizeof(pixel_t));
    for (int i = 0; i < header->height*header->width; i++) {
        image[i] = 255 - image [i];
    }
}

void rotate_left_90(pixel_t *image, header_t *header) {
    int a = header->width;
    header->width = header->height;
    header->height = a;
    pixel_t *rotate = malloc((header->width) * header->height * sizeof(pixel_t));

    for (int i = 0; i < header->height; ++i) {
        for (int j = 0; j < header->width; ++j) {
            pixel_t temp = image[header->width * i + j];
            rotate[abs(header->width) * (j + 1) - i - 1] = temp;
        }
    }
    image = rotate;
}
//
// Created by Людмила on 12.02.2020.
//

#ifndef COMP_GEOM_WORK_WITH_PNM_H
#define COMP_GEOM_WORK_WITH_PNM_H

#include <stdint.h>

typedef struct pixel_t {
    unsigned char b, g, r;
} pixel_t;


typedef struct header_t {
    char type_pnm;
    int type_pnm2;
    int width;
    int height;
    int deepth;
} header_t;

void from_pnm_color(FILE *input, pixel_t *read, header_t header);

void get_header(FILE *input, header_t *head);

void to_pnm_color(FILE *output, pixel_t *write, header_t write_header);

void to_pnm_gray(FILE *output, pixel_t *write, header_t write_header);

void from_pnm_gray(FILE *input, pixel_t *read, header_t header);

void inverse_color(pixel_t *image, header_t *header);

void rotate_left_90_gray(char *image, header_t *header);

void mirror_horizontal(pixel_t *image, header_t *header);

void mirror_horizontal_gray(unsigned char *image, header_t *header);

void mirror_vertical(pixel_t *image, header_t *header);

void mirror_vertical_gray(unsigned char *image, header_t *header);

void inverse_gray(unsigned char *image, header_t *header);

void rotate_left_90(pixel_t *image, header_t *header);

void rotate_right_90(struct pixel_t *image, header_t *header);

void rotate_right_90_gray(unsigned char *image, header_t *header);


#endif //COMP_GEOM_WORK_WITH_PNM_H

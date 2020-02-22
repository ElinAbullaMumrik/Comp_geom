//
// Created by Людмила on 12.02.2020.
//

#ifndef COMP_GEOM_WORK_WITH_PNM_H
#define COMP_GEOM_WORK_WITH_PNM_H

#include <stdint.h>


typedef enum {
    READ_OK = 0,
    READ_FILE_NOT_FOUND,
    READ_INVALID_BITS,
    READ_INVALID_HEADER,
    READ_FILENAME_NOT_FOUND
} read_status;

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

typedef struct pnm_picture_t {
    struct header_t h;
    pixel_t *data;
} pnm_picture_t;

void from_pnm(FILE *input, pnm_picture_t *read);

void get_header(FILE *input, header_t *head);

void to_pnm(FILE *output, pnm_picture_t *write);

pnm_picture_t rotate_left_90(pnm_picture_t image);

#endif //COMP_GEOM_WORK_WITH_PNM_H

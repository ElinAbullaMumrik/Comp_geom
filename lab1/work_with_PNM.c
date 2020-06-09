//
// Created by Людмила on 12.02.2020.
//
#include <stdio.h>
#include "work_with_PNM.h"
#include <stdlib.h>

void get_header(FILE *input, header_t *head) {
    fscanf(input, "%c%i%i%i%i\n", &head->type_pnm, &head->type_pnm2, &head->width, &head->height, &head->deepth);
//    return READ_OK;
}

void from_pnm_color(FILE *input, pixel_t *read, header_t header) {
    fread(read, 1, header.width * header.height * sizeof(struct pixel_t), input);//    return READ_OK;
}

void from_pnm_gray(FILE *input, unsigned char *read, header_t header) {
    fread(read, 1, header.width * header.height * sizeof(unsigned char), input);//    return READ_OK;
}

void to_pnm_color(FILE *output, pixel_t *write, header_t write_header) {
    fprintf(output, "%c%i\n%i %i\n%i\n",
            write_header.type_pnm, write_header.type_pnm2, write_header.width, write_header.height,
            write_header.deepth);
    fwrite(write, 1, write_header.width * write_header.height * sizeof(pixel_t), output);
}

void to_pnm_gray(FILE *output, unsigned char *write, header_t write_header) {
    fprintf(output, "%c%i\n%i %i\n%i\n",
            write_header.type_pnm, write_header.type_pnm2, write_header.width, write_header.height,
            write_header.deepth);
    fwrite(write, 1, write_header.width * write_header.height * sizeof(unsigned char), output);
}

void inverse_color(pixel_t *image, header_t *header) {
//    pixel_t *inversed = malloc((header->width) * header->height * sizeof(pixel_t));
    for (int i = 0; i < header->height * header->width; i++) {
        pixel_t temp = image[i];
        temp.b = 255 - temp.b;
        temp.g = 255 - temp.g;
        temp.r = 255 - temp.r;
        image[i] = temp;
    }
}

void inverse_gray(unsigned char *image, header_t *header) {
    for (int i = 0; i < header->height * header->width; i++) {
        image[i] = 255 - image[i];
    }
}

void mirror_horizontal_gray(unsigned char *image, header_t *header) {
    unsigned char *mirror = (unsigned char *) malloc(header->width * header->height * sizeof(unsigned char));
    if (!mirror) {
        printf("Error: troubles with memory\n");
        return;
    }
    for (int i = 0; i < header->height; ++i) {
        for (int j = 0; j < header->width; ++j) {
            mirror[i * header->width + j] = image[i * header->width + header->width - j - 1];
        }
    }
    for (int i = 0; i < header->height * header->width; i++) {
        image[i] = mirror[i];
    }
    free(mirror);
}

void mirror_horizontal(pixel_t *image, header_t *header) {
    pixel_t *mirror = (pixel_t *) malloc(header->width * header->height * sizeof(pixel_t));
    if (!mirror) {
        printf("Error: troubles with memory\n");
        return;
    }
    for (int i = 0; i < header->height; ++i) {
        for (int j = 0; j < header->width; ++j) {
            mirror[i * header->width + j] = image[i * header->width + header->width - j - 1];
        }
    }
    for (int i = 0; i < header->height * header->width; i++) {
        image[i] = mirror[i];
    }
    free(mirror);
}

void mirror_vertical_gray(unsigned char *image, header_t *header) {
    unsigned char *mirror = (unsigned char *) malloc(header->width * header->height * sizeof(unsigned char));
    if (!mirror) {
        printf("Error: troubles with memory\n");
        return;
    }
    for (int i = 0; i < header->height; ++i) {
        for (int j = 0; j < header->width; ++j) {
            mirror[i * header->width + j] = image[(header->height - i) * header->width + j];
        }
    }
    for (int i = 0; i < header->height * header->width; i++) {
        image[i] = mirror[i];
    }
    free(mirror);
}

void mirror_vertical(pixel_t *image, header_t *header) {
    pixel_t *mirror = (pixel_t *) malloc(header->width * header->height * sizeof(pixel_t));
    if (!mirror) {
        printf("Error: troubles with memory\n");
        return;
    }
    for (int i = 0; i < header->height; ++i) {
        for (int j = 0; j < header->width; ++j) {
            mirror[i * header->width + j] = image[(header->height - i) * header->width + j];
        }
    }
    for (int i = 0; i < header->height * header->width; i++) {
        image[i] = mirror[i];
    }
    free(mirror);
}


void rotate_left_90_gray(unsigned char *image, header_t *header) {
    int a = header->width;
    header->width = header->height;
    header->height = a;
    unsigned char *rotate = (unsigned char *) malloc((header->width) * header->height * sizeof(unsigned char));
    if (!rotate) {
        printf("Error: troubles with memory\n");
        return;
    }
    for (int i = 0; i < header->width; ++i) {
        for (int j = 0; j < header->height; ++j) {
            unsigned char temp = image[header->height * i + j];
            rotate[(header->height - j - 1) * (header->width) + i] = temp;
        }
    }
    for (int i = 0; i < header->height * header->width; i++) {
        image[i] = rotate[i];
    }
    free(rotate);
}

void rotate_left_90(pixel_t *image, header_t *header) {
    int a = header->width;
    header->width = header->height;
    header->height = a;
    pixel_t *rotate = (pixel_t *) malloc((header->width) * header->height * sizeof(pixel_t));
    if (!rotate) {
        printf("Error: troubles with memory\n");
    }
    for (int i = 0; i < header->width; ++i) {
        for (int j = 0; j < header->height; ++j) {
            pixel_t temp = image[header->height * i + j];
            rotate[(header->height - j - 1) * (header->width) + i] = temp;
        }
    }
    for (int i = 0; i < header->height * header->width; i++) {
        image[i] = rotate[i];
    }
    free(rotate);
}

void rotate_right_90_gray(unsigned char *image, header_t *header) {
    int a = header->width;
    header->width = header->height;
    header->height = a;
    unsigned char *rotate = (unsigned char *) malloc((header->width) * header->height * sizeof(unsigned char));
    if (!rotate) {
        printf("Error: troubles with memory\n");
        return;
    }
    for (int i = 0; i < header->width; ++i) {
        for (int j = 0; j < header->height; ++j) {
            unsigned char temp = image[header->height * i + j];
            rotate[j * (header->width) + header->width - i] = temp;
//            rotate[(header->height - j - 1) * (header->width) + i] = temp;
        }
    }
    for (int i = 0; i < header->height * header->width; i++) {
        image[i] = rotate[i];
    }
    free(rotate);
}

void rotate_right_90(struct pixel_t *image, header_t *header) {
    int a = header->width;
    header->width = header->height;
    header->height = a;
    pixel_t *rotate = (pixel_t *) malloc((header->width) * header->height * sizeof(pixel_t));
    if (!rotate) {
        printf("Error: troubles with memory\n");
        return;
    }
    for (int i = 0; i < header->width; ++i) {
        for (int j = 0; j < header->height; ++j) {
            pixel_t temp = image[header->height * i + j];
            rotate[j * (header->width) + header->width - i] = temp;
        }
    }
    for (int i = 0; i < header->height * header->width; i++) {
        image[i] = rotate[i];
    }
    free(rotate);
}
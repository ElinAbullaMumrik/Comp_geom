#include <stdio.h>
#include <stdlib.h>
#include "work_with_PNM.h"

int main() {
    char const *filename = "s.pnm";

    header_t *header = (header_t *) malloc(sizeof(header_t));
    FILE *input = fopen(filename, "rb");
    FILE *outptr = fopen("r.pnm", "wb");
    get_header(input, header);
    if (header->type_pnm2 == 5){

    } else if (header->type_pnm2 ==6){

    } else{
        return "все плохо";
    }
    pixel_t *picture = (pixel_t *) malloc(sizeof(pixel_t) * header->height * header->width);
    from_pnm_color(input, picture, *header);
    printf("%s", picture);
    picture = rotate_left_90(picture, *header);
    to_pnm(outptr, picture, *header);
    fclose(outptr);
    return 0;
}

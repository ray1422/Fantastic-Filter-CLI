#include "jpg_info.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/stb_image.h"
#include "../lib/stb_image_resize.h"
#include "../lib/stb_image_write.h"

/*
 * @param jpg_filename
 * @return int[2] {height, width}
 */
int *jpg_info(char *jpg_filename) {
    // TODO Optimize proformance
    int width = 0, height = 0, bpp = 0;
    uint8_t *rgb_image = stbi_load(jpg_filename, &width, &height, &bpp, 3);
    if (rgb_image == NULL) return NULL;
    stbi_image_free(rgb_image);
    static int size[2];
    size[0] = height;
    size[1] = width;
    // printf("size: %d, %d\n", height, width);
    int *ptr = size;
    return ptr;
}
#include "FF.h"
#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "lib/stb_image_write.h"

int main() {
    FF* ff = FF_get_instance();
    int width, height, bpp;
    uint8_t* rgb_image = stbi_load("www.jpg", &width, &height, &bpp, 3);
    int output_height = 0, output_width = 0;
    uint8_t* result = NULL;
    FF_enhance(FF_get_instance(), rgb_image, width, height, &result, &output_width, &output_height);
    FF_close_instance();
    stbi_image_free(rgb_image);
    stbi_write_jpg("image.jpg", output_width, output_height, 3, result, 100);
}
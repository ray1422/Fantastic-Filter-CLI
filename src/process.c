#include "process.h"

#include "FF.h"

// #define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"

// #define STB_IMAGE_WRITE_IMPLEMENTATION
#include "lib/stb_image_resize.h"
#include "lib/stb_image_write.h"

error_t process(uint8_t *data, int height, int width, uint8_t **result, int *output_height, int *output_width) {
    FF *ff = FF_get_instance();
    if (ff == NULL) return PSTAT_NULLPTR;
    FF_enhance(ff, data, width, height, result, output_height, output_width);
    return PSTAT_OK;
}

error_t process_file(const char *input_filename, uint8_t **result, int *output_height, int *output_width) {
    int width = 0, height = 0, bpp = 3;
    uint8_t *data = stbi_load(input_filename, &width, &height, &bpp, 3);
    if (data == NULL) return PSTAT_FILE_NOT_FOUND;
    error_t stat = process(data, height, width, result, output_height, output_width);
    stbi_image_free(data);
    return stat;
}

error_t process_file_and_resize(const char *input_filename, int height, int width, uint8_t **result, int *output_height, int *output_width) {
    int _width = 0, _height = 0, bpp = 3;
    uint8_t *data = stbi_load(input_filename, &_width, &_height, &bpp, 3);
    uint8_t *data_new = malloc(height * width * 3 * sizeof(uint8_t));
    if (data == NULL || data_new == NULL) return PSTAT_FILE_NOT_FOUND;
    if (_width != width || _height != height || bpp != 3) {
        // resize
        int stat = stbir_resize_uint8(data, _width, _height, 0, data_new, width, height, 0, 3);
        if (!stat) return PSTAT_OOM;
        stbi_image_free(data);
        data = data_new;
    }
    error_t stat = process(data, height, width, result, output_height, output_width);
    stbi_image_free(data);
    return stat;
}

error_t process_file_and_save(const char *input_filename, const char *output_filename) {
    uint8_t *result = NULL;
    int height = 0, width = 0;
    error_t stat = process_file(input_filename, &result, &height, &width);
    if (stat == PSTAT_OK) {
        stbi_write_jpg(output_filename, height, width, 3, result, 95);
    }
    return stat;
}

error_t process_file_resize_and_save(const char *input_filename, int height, int width, const char *output_filename) {
    uint8_t *result = NULL;
    int _height = 0, _width = 0;
    // error_t stat = process_file(input_filename, &result, &_height, &_width);
    error_t stat = process_file_and_resize(input_filename, height, width, &result, &_height, &_width);
    if (stat == PSTAT_OK) {
        stbi_write_jpg(output_filename, _height, _width, 3, result, 95);
    }
    return stat;
}
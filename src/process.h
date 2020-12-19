#ifndef PROCESS_H
#define PROCESS_H
#include <bits/types/error_t.h>
#include <stdint.h>

typedef enum process_stat {
    PSTAT_OK,
    PSTAT_NULLPTR,
    PSTAT_FILE_NOT_FOUND,
    PSTAT_PROCESS_ERROR,
    PSTAT_OOM
} process_stat_t;

/* 
 * Process with uint_8 array. 
 * @param   image_data_ptr
 * @return  error_code
 */
error_t process(uint8_t *data, int height, int width, uint8_t **result, int *output_height, int *output_width);

/*
 * Process a jpg file. 
 * @param   input_filename
 * @param   result
 * @param   output_height
 * @param   output_width
 * @return  error_code
 */
error_t process_file(const char *input_filename, uint8_t **result, int *output_height, int *output_width);

/*
 * Process a jpg file. 
 * @param   input_filename
 * @param   height
 * @param   width
 * @param   result
 * @param   output_height
 * @param   output_width
 * @return  error_code
 */
error_t process_file_and_resize(const char *input_filename, int height, int width, uint8_t **result, int *output_height, int *output_width);

/*
 * @param   input_filename
 * @param   output_filename
 * @return  error_code
 */
error_t process_file_and_save(const char *input_filename, const char *output_filename);

/*
 * @param   input_filename
 * @param   height
 * @param   width
 * @param   output_filename
 * @return  error_code
 */
error_t process_file_resize_and_save(const char *input_filename, int height, int width, const char *output_filename);

#endif

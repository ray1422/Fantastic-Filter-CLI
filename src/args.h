#ifndef ARGS_H
#define ARGS_H
#include "utils/vector.h"

typedef struct args {
    Vector input_files;   // args
    char *input_dir;      // -i
    char *output_dir;     // -o
    int silent, verbose;  // -s -v
    unsigned int max_height, max_width;
    int benchmark;  // -b
    int benchmark_level;  // -b
} Args;

extern Args FF_arg;
Args *args_init(int argc, char *argv[]);
void args_free(Args *args);

#endif

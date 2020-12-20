#include "args.h"

#include <argp.h>
#include <stdlib.h>

#include "utils/color.h"
#include "utils/vector.h"

Args FF_arg;

static void _args_init() {
    Args *args = &FF_arg;
    args->input_files = vector_new(char *);
    args->input_dir = NULL;
    args->output_dir = "./";
    args->silent = 0;
    args->verbose = 0;
    args->benchmark = 0;
    args->benchmark_level = -1;
    args->max_width = 0;
    args->max_height = 0;
}

/* 
 *  @param key
 *  @param argv
 *  #param state
 */
static error_t parse_opt(int key, char *argv, struct argp_state *state) {
    struct args *args = state->input;
    switch (key) {
        case 'w':
            if (argv == NULL || atoi(argv) <= 0) {
                fputs(C_WHT "Invaild " C_BRED "max-width" C_RESET C_WHT "!\n" C_RESET, stderr);
                argp_usage(state);
            }
            args->max_width = atoi(argv);
            break;
        case 'h':
            if (argv == NULL || atoi(argv) <= 0) {
                fputs(C_WHT "Invaild " C_BRED "max-height" C_RESET C_WHT "!\n" C_RESET, stderr);
                argp_usage(state);
            }
            args->max_height = atoi(argv);
            break;
        case 'q':
        case 's':
            args->silent = 1;
            fclose(stdout);
            break;
        case 'v':
            args->verbose = 1;
            break;
        case 'i':
            args->input_dir = argv;
            fputs(C_BRED "input_dir" C_RESET C_WHT " option is unimplemented!\n" C_RESET, stderr);
            return ARGP_ERR_UNKNOWN;
            break;
        case 'o':
            args->output_dir = argv;
            break;
        case 'b':
            args->benchmark = 1;
            if (argv != NULL) {
                if (*argv == '=') argv += 1;
                args->benchmark_level = atoi(argv);
            }
        case ARGP_KEY_ARG:
            vector_push_back(args->input_files, argv);

            break;

        case ARGP_KEY_END:
            if (args->input_dir == NULL && state->arg_num < 1 && !args->benchmark) {
                /* Not enough arguments. */
                fputs("Please set either input_files or input_folder!\n", stderr);
                argp_usage(state);
            }
            fclose(stderr);
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

const char *argp_program_version = "Fantastilc-Filter-CLI 0.0.1-alpha";
const char *argp_program_bug_address = "<ray1422@csie.io>";

Args *args_init(int argc, char *argv[]) {
    static char doc[] =
        "Fantastic Filter CLI - A deeplearning based filter for night shot photos.\n"
        "set environment variable " C_BWHT "FF_MODEL_PATH" C_RESET " to the model's path.";
    static struct argp_option options[] = {
        {"input_dir", 'i', "DIR", OPTION_HIDDEN, "Input directory. input_files would be ignored if input_dir is set."},
        {"output_dir", 'o', "DIR", 0, "Output directory. Default to current folder."},
        {"max-width", 'w', "MAX_WIDTH", 0, "Max width. Images larger than this would be resize."},
        {"max-height", 'h', "MAX_HEIGHT", 0, "Max height. Images larger than this would be resize."},
        {"quiet", 'q', 0, 0, "Don't produce any output."},
        {"silent", 's', 0, OPTION_ALIAS},
        {"verbose", 'v', 0, 0, "Show verbose"},
        {"benchmark", 'b', "LEVEL", OPTION_ARG_OPTIONAL, "Run Fantastic Filter Benchmark"},

        {0},  // input_files
    };
    static struct argp argp = {options, parse_opt, "[input_files]", doc};

    _args_init();
    argp_parse(&argp, argc, argv, 0, 0, &FF_arg);
    // for (int i = 0; i < args.input_files.size; i++) {
    //     puts(vector_get(args.input_files, i, char *));
    // }
    return &FF_arg;
}

void args_free(Args *args) {
    vector_destory(args->input_files, NULL);
}
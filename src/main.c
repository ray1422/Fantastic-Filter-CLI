#include <assert.h>
#include <libgen.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <tensorflow/c/c_api.h>
#include <time.h>
#include <unistd.h>

#include "FF.h"
#include "args.h"
#include "benchmark.h"
#include "process.h"
#include "utils/color.h"
#include "utils/debug.h"
#include "utils/jpg_info.h"

typedef struct Img {
    int width;
    int height;
    int need_resize;
    char *path;
} Img;

static int img_cmp(const void *_a, const void *_b) {
    Img *a = (Img *)_a, *b = (Img *)_b;
    if (a->height == b->height) return a->width > b->width;
    return a->height > b->height;
}

static void *print_progress_bar(void *_n) {
    long long milliseconds = 500;
    int n = *(int *)_n;
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000 / n;
    ts.tv_nsec = (milliseconds % 1000) * 1000000 / n;

    for (int i = 0; i < n; i++) {
        putchar('=');
        fflush(stdout);
        // nanosleep(&ts, NULL);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int n_success = 0, n_failed = 0;
    args_init(argc, argv);
    if (FF_arg.benchmark) {
        return benchmark_main();
    }
    Vector *files = &FF_arg.input_files;
    Img images[files->size + 5];
    // check each files & keep size info. maybe needs to calc resized size
    int n_images = 0;
    for (int i = 0; i < files->size; i++) {
        int *size = jpg_info(vector_get(*files, i, char *));
        if (size == NULL) {
            printf("Opening " C_BWHT C_UWHT "%s" C_RESET C_BRED " failed!\n" C_RESET, vector_get(*files, i, char *));
            n_failed++;
            continue;
        }
        int height = size[0], width = size[1];
        // printf("size: %d x %d\n", height, width);
        images[i].need_resize = 1;
        double ratio = 1.;
        if (FF_arg.max_height && FF_arg.max_width && width > FF_arg.max_width && height > FF_arg.max_height) {
            // both height and width are set
            images[i].need_resize = 1;
            double r_width = FF_arg.max_width / width;
            double r_height = FF_arg.max_height / height;
            ratio = r_width < r_height ? r_width : r_height;
        } else if (FF_arg.max_width && width > FF_arg.max_width) {
            ratio = (double)FF_arg.max_width / width;
        } else if (FF_arg.max_height && height > FF_arg.max_height) {
            ratio = (double)FF_arg.max_height / height;
        } else {
            images[i].need_resize = 0;
        }
        width = (double)width * ratio;
        height = (double)height * ratio;
        if (height % 8 || width % 8) {
            images[i].need_resize = 1;
            height -= height % 8;
            width -= width % 8;
        }
        if (height < 64 || width < 64) {
            printf("%s with size (%4d, %4d) is too small! ignored.\n",
                   vector_get(*files, i, char *), height, width);
            continue;
        }
        images[n_images].width = width;
        images[n_images].height = height;
        images[n_images].path = vector_get(*files, i, char *);
        n_images++;
    }

    // sort (group) with size for better buffering.
    qsort(images, n_images, sizeof(images[0]), img_cmp);

    // start processing.
    if (n_images == 1) {
        printf("There is " C_BWHT "%d" C_RESET " image need to be processed.\n", n_images);
    } else {
        printf("There are " C_BWHT "%d" C_RESET " images need to be processed.\n", n_images);
    }

    for (int i = 0; i < n_images; i++) {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        char *filename = images[i].path;
        char dist_filename[8192] = {'\0'};
        // todo show error msg
        assert(strlen(filename) + strlen(FF_arg.output_dir) < 4096);

        strcat(dist_filename, FF_arg.output_dir);
        strcat(dist_filename, "/");
        strcat(dist_filename, "enhanced_");
        strcat(dist_filename, basename(images[i].path));

        if (strlen(filename) - w.ws_col < 15) filename = basename(images[i].path);
        printif(FF_arg.verbose, "processing ");
        printif(!FF_arg.silent, C_BWHT "%s" C_RESET " ", filename);
        printif(FF_arg.verbose, "\n");

        int n_dots = w.ws_col - strlen(filename) - strlen(basename(dist_filename)) - 25;
        pthread_t tid;
        if (!FF_arg.verbose && !FF_arg.silent) pthread_create(&tid, NULL, print_progress_bar, &n_dots);
        error_t stat;
        if (images[i].need_resize) {
            stat = process_file_resize_and_save(images[i].path, images[i].height, images[i].width, dist_filename);
        } else {
            stat = process_file_and_save(images[i].path, dist_filename);
        }

        if (!FF_arg.verbose && !FF_arg.silent) {
            pthread_join(tid, NULL);
            fputs("> ", stdout);
        }

        if (stat == PSTAT_OK) {
            printf(C_BWHT "%s" C_RESET " ", basename(dist_filename));
            printf(C_BGRN " %s" C_RESET "\n", "Done");
            n_success++;
        } else {
            printf(C_BHBLK "%s" C_RESET "", basename(dist_filename));
            printf(C_BRED " %s" C_RESET "\n", "Failed");
            if (stat == PSTAT_NULLPTR) {
                puts("error occurred. Is " C_BWHT "FF_MODEL_PATH" C_RESET " set correctly?");
            } else if (stat == PSTAT_OOM) {
                puts("OOM! Please buy more high-end graphis cards.");
            } else {
                puts("Unknown error! Maybe try to run with --verbose?");
            }
            n_failed++;
        }
    }
    printf("All done! There are " C_BGRN "%d" C_RESET " successfully processed and " C_BRED "%d" C_RESET " failed.\n", n_success, n_failed);
    puts("Support us on " C_BWHT C_UWHT "https://ray-fish.me/Fantastic-Filter-Professional-Plus/" C_RESET " !");
    args_free(&FF_arg);
    FF_close_instance();
}

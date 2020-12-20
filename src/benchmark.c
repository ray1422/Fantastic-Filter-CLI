#include "benchmark.h"

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "FF.h"
#include "args.h"
#include "utils/color.h"

// benchmark resolution level
const int LEVELS[][3] = {
    {128, 128, 3},
    {1280, 768, 3},
    {1920, 1080, 3},
    {2560, 1440, 3},
    {3840, 2160, 3},
};

uint8_t data[4000 * 4000 * 3];

typedef struct _msg {
    int total;
    int n_processed;
    // benchmark done singal
    volatile int done;
    // await ascii-art
    pthread_mutex_t lock;
} Msg;

static void *progressbar(void *_stat) {
    struct timeval t1, t2;
    int sec, min;
    gettimeofday(&t1, NULL);

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    if (!getenv("I_DONT_WANNA_SEE_ASCIIART")) {
        if (w.ws_col <= 120) {
            puts(C_RED "Your terminal size is too small, please resize your terminal or set enviorment variable I_DONT_WANNA_SEE_ASCIIART\n\n" C_RESET);
        }
        // print logo
        int last_win_size = w.ws_col;
        while (w.ws_col <= 120 && last_win_size == w.ws_col) {
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
            sleep(1);
            last_win_size = w.ws_col;
        }
        if (w.ws_col > 120) {  // screen width
            puts(ASCIIART_LOGO);
        }
    }
    const int delay_ms = 1000 / 24;
    unsigned int idc_idx = 0;
    Msg *msg = (Msg *)_stat;
    pthread_mutex_unlock(&msg->lock);
    // const char idc[][10] = {"⠟", "⠯", "⠷", "⠾", "⠽", "⠻"};
    const char idc[][10] = {"⠉", "⠯", "⠷", "⠾", "⠽", "⠻"};

    while (1) {
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        gettimeofday(&t2, NULL);
        sec = (int)(t2.tv_sec - t1.tv_sec);
        min = sec / 60;
        sec %= 60;

        int progress_length = 90;
        if (w.ws_col < 114) progress_length = w.ws_col - 30;
        int progress_filled = progress_length * (msg->n_processed / (double)msg->total);
        int padding_left = 4;
        for (int i = 0; i < padding_left; i++) putchar(' ');
        fputs(idc[idc_idx], stdout);
        idc_idx++;
        idc_idx %= (sizeof(idc) / sizeof(idc[0]));
        printf("  %5.2lf%%", msg->n_processed / (double)msg->total * 100);

        fputs("  " C_BHWHT, stdout);
        for (int i = 0; i < progress_filled; i++) fputs("▉", stdout);
        fputs(C_BHBLK, stdout);
        for (int i = 0; i < progress_length - progress_filled; i++) fputs("▉", stdout);
        fputs(C_RESET "  ", stdout);
        printf("%02d:%02d  ", min, sec);

        fflush(stdout);
        printf("\r");
        struct timespec ts;

        // sleep
        ts.tv_sec = delay_ms / 1000;
        ts.tv_nsec = (delay_ms % 1000) * 1000000;
        if (msg->done) {
            puts("");
            break;
        }
        nanosleep(&ts, NULL);
    }
    //  30%  ▉▉▉▉▉▉▉▉▒▒▒▒▒▒▒▒▒▒▒▒▒▒  (MM:SS)
    return NULL;
}

int benchmark(int level) {
    const double K = 80.0;
    const int N = 95;
    int failed = 0;
    // init test image.
    // test once but don't calc first time costs.
    // run N times.
    Msg msg = {N, 0, 0, PTHREAD_MUTEX_INITIALIZER};
    pthread_t tid;
    pthread_mutex_lock(&msg.lock);
    // boostrap UI thread
    pthread_create(&tid, NULL, progressbar, &msg);
    FF *ff = FF_get_instance();
    if (ff == NULL) {
        msg.done = 1;
        pthread_join(tid, NULL);
        puts("\n" C_BRED "Failed!" C_RESET " error occurred. Is " C_BWHT "FF_MODEL_PATH" C_RESET " set correctly?");
        failed = 1;
    }
    uint8_t *result = NULL;
    int _unused;
    FF_enhance(ff, data, LEVELS[level][1], LEVELS[level][0], &result, &_unused, &_unused);
    // await for ascii-arts to print
    pthread_mutex_lock(&msg.lock);
    pthread_mutex_destroy(&msg.lock);
    struct timeval ts, te;
    gettimeofday(&ts, NULL);
    for (int i = 0; i < N; i++) {
        // printf("[%2d / %2d]\t\t", i, N);
        FF_enhance(ff, data, LEVELS[level][1], LEVELS[level][0], &result, &_unused, &_unused);
        msg.n_processed++;
        // fputs("done.\r", stdout);
        // fflush(stdout);
    }
    msg.done = 1;
    gettimeofday(&te, NULL);
    // await UI thread
    pthread_join(tid, NULL);
    // Calcute score.
    if (failed) {
        return 1;
    }
    double time_in_mill = (te.tv_sec - ts.tv_sec) * 1000 + (double)(te.tv_usec - ts.tv_usec) / 1000;
    puts("");
    double score = K / time_in_mill * LEVELS[level][0] * LEVELS[level][1];
    printf("Your score:\t" C_GRN "%.2lf" C_RESET " pts (level %d)\n", score, level);
    return -1;
}

int benchmark_main() {
    printf("\033[H\033[J");
START:;
    int level = FF_arg.benchmark_level;
    puts("Benchmark levels:");
    for (int i = 0; i < sizeof(LEVELS) / sizeof(LEVELS[0]); i++) {
        printf("\t[%2d] (%4d x %4d)\n", i, LEVELS[i][0], LEVELS[i][1]);
    }
    if (level == -1 || level < 0 || level >= sizeof(LEVELS) / sizeof(LEVELS[0])) {
        printf("Enter your choice [0-%lu]: ", sizeof(LEVELS) / sizeof(LEVELS[0]) - 1);
        int flg = scanf("%d", &level);
        if (flg == EOF) {
            puts(C_BYEL "::EOF::" C_RESET);
            puts("Good bye.");
            return 0;
        }
    }
    puts("");
    if (level < 0 || level >= sizeof(LEVELS) / sizeof(LEVELS[0])) {
        puts("");
        puts(C_BRED "Invaild input!" C_RESET);

        for (char c; (c = getchar() != '\n' && c != EOF);)
            ;

        goto START;
    }
    printf("Starting benchmark [level: %d]\n\n", level);
    benchmark(level);
    return 0;
}

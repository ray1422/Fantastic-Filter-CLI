#ifndef DEBUG
#include <stdio.h>
#ifdef _DEBUG
#define DEBUG(format, args...) fprintf(stderr,                                  \
                                       "\033[1;34m"                             \
                                       "[%s:%3d] \033[1;33m" format "\033[m\n", \
                                       __FILE__, __LINE__, ##args)
#else
#define DEBUG(args...)
#endif
#define printif(cond, args...) \
    do {                       \
        if (cond) {            \
            printf(args);      \
        }                      \
    } while (0);
#endif
#include "test.h"
static void fn(void) __attribute__((constructor));
static void fn() {
    test_init();
    // puts("hello2!");
}
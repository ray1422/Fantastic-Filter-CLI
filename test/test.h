#ifndef TEST_H
#define TEST_H
#include <stdbool.h>
#include <stdio.h>

#include "utils/color.h"
#include "utils/vector.h"
extern int test_n_passed;
extern int test_n_failed;
extern Vector tests;
extern const char *STATUS;
#define TEST_STATUS_INIT const char *TEST_STATUS[] = {C_BRED "FAILED" C_RESET, C_BGRN "PASSED" C_RESET, ""};
#define EXPECT_EQ(PREDICT, TARGET)                                                                               \
    {                                                                                                            \
        TEST_STATUS_INIT;                                                                                        \
        bool stat = ((PREDICT) == (TARGET));                                                                     \
        test_n_passed += stat;                                                                                   \
        test_n_failed += !stat;                                                                                  \
        do {                                                                                                     \
            fprintf(stderr, "%s " C_BWHT #PREDICT C_RESET " == " C_BWHT #TARGET C_RESET "\n", TEST_STATUS[stat]); \
        } while (0);                                                                                             \
    }

#define EXPECT_NOT_EQ(PREDICT, TARGET)                                                                            \
    {                                                                                                             \
        TEST_STATUS_INIT;                                                                                         \
        bool stat = ((PREDICT) != (TARGET));                                                                      \
        test_n_passed += stat;                                                                                    \
        test_n_failed += !stat;                                                                                   \
        do {                                                                                                      \
            fprintf(stderr, "%s " C_BWHT #PREDICT C_RESET " != " C_BWHT #TARGET C_RESET "\n", TEST_STATUS[stat]); \
        } while (0);                                                                                              \
    }

void test_init();

#endif
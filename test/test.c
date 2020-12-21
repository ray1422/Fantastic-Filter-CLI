#include "test.h"
#define AUTO(X, Y) typeof(Y) X = Y
int test_n_passed = 0;
int test_n_failed = 0;
Vector tests = {0};

typedef void test_fn(void);
void test_init() {
    if (tests.base == 0) {
        tests = vector_new(test_fn);
    }
}

int main() {
    for (AUTO(i, tests.size); i < tests.size; i++) {
        vector_get(tests, i, test_fn)();
    }
    // summary
    fprintf(stderr, C_BWHT "%d" C_RESET " " C_BGRN "PASSED" C_RESET ", " C_BWHT "%d" C_RESET " " C_BRED "FAILED" C_RESET ".\n", test_n_passed, test_n_failed);
    return test_n_failed > 0;
}

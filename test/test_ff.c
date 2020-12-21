#include <assert.h>

#include "../src/FF.h"
#include "test.h"
static void fn(void) __attribute__((constructor));
static void fn() {
    EXPECT_NOT_EQ(FF_get_instance(), NULL);
    int EXPECT_WIDTH = 128, EXPECT_HEIGHT = 128;
    assert(EXPECT_WIDTH % 8 == 0 && EXPECT_HEIGHT % 8 == 0);
    uint8_t data[EXPECT_WIDTH * EXPECT_HEIGHT * 3];
    uint8_t *result;
    int height = 0, width = 0;
    EXPECT_EQ(FF_enhance(FF_get_instance(), data, EXPECT_WIDTH, EXPECT_HEIGHT, &result, &width, &height), FF_OK);
    EXPECT_EQ(EXPECT_HEIGHT, height);
    EXPECT_EQ(EXPECT_WIDTH, width);
    FF_close_instance();
}
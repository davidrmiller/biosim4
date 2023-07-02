#include "../src/basicTypes.h"
#include <gtest/gtest.h>


TEST(CommonTests, TestCompass) {
    
    BS::Compass compass = BS::Compass(BS::Compass::SW);

    EXPECT_EQ(0, (uint8_t)compass);
}

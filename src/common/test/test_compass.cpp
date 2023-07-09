#include "../include/compass.h"

#include <gtest/gtest.h>


TEST(CompassTests, TestCompass) {
    
    BS::Compass compass = BS::Compass(BS::Compass::SW);

    EXPECT_EQ(0, (uint8_t)compass);
}

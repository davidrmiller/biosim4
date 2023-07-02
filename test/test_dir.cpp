#include "../src/basicTypes.h"
#include <gtest/gtest.h>

namespace BS {

TEST(CommonTests, TestDirAsInt) {
    
    Dir d1 = Dir(Compass::N);

    EXPECT_EQ(d1.asInt(), (int)Compass::N);

    EXPECT_NE(d1.asInt(), 0);
}

TEST(CommonTests, TestDirEquality) {

    EXPECT_EQ(Dir(Compass::W), Compass::W);

    Dir d3 = Compass::N;
    Dir d4 = Compass::N;
    EXPECT_EQ(d3, d4);

    Dir d5 = Compass::NE;
    EXPECT_NE(d3, d5);
}

TEST(CommonTests, TestDirRotate) {

    Dir d1 = Compass::NE;
    // rotation happens in 45 degree increments, positive is clockwise
    EXPECT_EQ(d1.rotate(1), Compass::E);
    EXPECT_EQ(d1.rotate(2), Compass::SE);
    EXPECT_EQ(d1.rotate(-1), Compass::N);
    EXPECT_EQ(d1.rotate(-2), Compass::NW);
    EXPECT_EQ(Dir(Compass::N).rotate(1), d1);
    EXPECT_EQ(Dir(Compass::SW).rotate(-2), Compass::SE);
}

}
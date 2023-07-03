#include "../src/common/dir.h"
#include "../src/common/compass.h"



#include <gtest/gtest.h>

namespace BS {

TEST(DirTests, TestDirAsInt) {
    
    Dir d1 = Dir(Compass::N);

    EXPECT_EQ(d1.asInt(), (int)Compass::N);

    EXPECT_NE(d1.asInt(), 0);
}

TEST(DirTests, TestDirCopyAssignment) {
    
    Dir d1 = Dir(Compass::N);
    Dir d2 = Dir(Compass::CENTER);

    d1 = d2;

    EXPECT_EQ(d1.asInt(), (int)Compass::CENTER);

    EXPECT_NE(d1.asInt(), 0);
}

TEST(DirTests, TestDirEquality) {

    EXPECT_EQ(Dir(Compass::W), Compass::W);

    Dir d3 = Compass::N;
    Dir d4 = Compass::N;
    EXPECT_EQ(d3, d4);

    Dir d5 = Compass::NE;
    EXPECT_NE(d3, d5);
}

TEST(DirTests, TestDirRotate) {

    Dir d1 = Compass::NE;
    // rotation happens in 45 degree increments, positive is clockwise
    EXPECT_EQ(d1.rotate(1), Compass::E);
    EXPECT_EQ(d1.rotate(2), Compass::SE);
    EXPECT_EQ(d1.rotate(-1), Compass::N);
    EXPECT_EQ(d1.rotate(-2), Compass::NW);
    EXPECT_EQ(Dir(Compass::N).rotate(1), d1);
    EXPECT_EQ(Dir(Compass::SW).rotate(-2), Compass::SE);
}

TEST(DirTests, TestDirAsNormalisedCoord) {

    Coord c1 = Dir(Compass::CENTER).asNormalizedCoord();

    EXPECT_EQ(0, c1.x);
    EXPECT_EQ(0, c1.y);

    Dir d1 = Compass::SW;
    
    Coord c2 = d1.asNormalizedCoord();
    EXPECT_EQ(-1, c2.x);
    EXPECT_EQ(-1, c2.y);

}

TEST(DirTests, TestDirAsNormalizedPolar) {

    Dir d1 = Compass::SW;
    Polar p1 = d1.asNormalizedPolar();
    
    EXPECT_EQ(1, p1.mag);
    EXPECT_EQ(d1, p1.dir);

    Dir d2 = Compass::S;
    Polar p2 = d2.asNormalizedPolar();

    EXPECT_EQ(1, p2.mag);
    EXPECT_EQ(d2, p2.dir);

}

} // namespace BS
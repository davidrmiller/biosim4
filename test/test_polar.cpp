#include "../src/common/polar.h"
#include "../src/common/dir.h"
#include "../src/common/compass.h"

#include <gtest/gtest.h>

namespace BS {

TEST(PolarTests, TestPolarDefault) {
    
    Polar p1 = Polar();

    EXPECT_EQ(p1.mag, 0);
    EXPECT_EQ(p1.dir, Dir(Compass::CENTER));

}

TEST(PolarTests, TestPolarConstruct) {
    Polar p1 = Polar(-10, Compass::NW);
    EXPECT_EQ(p1.mag, -10);
    EXPECT_EQ(p1.dir, Dir(Compass::NW));
}

TEST(PolarTests, TestPolarAsCoord) {

    Coord c1 = Polar(0, Compass::CENTER).asCoord();
    EXPECT_EQ(c1.x, 0);
    EXPECT_EQ(c1.y, 0);

    c1 = Polar(10, Compass::CENTER).asCoord();
    EXPECT_EQ(c1.x, 0);
    EXPECT_EQ(c1.y, 0);
    
    c1 = Polar(20, Compass::N).asCoord();
    EXPECT_EQ(c1.x, 0);
    EXPECT_EQ(c1.y, 20);

    Polar p1 = Polar(12, Compass::W);
    c1 = p1.asCoord();
    EXPECT_EQ(c1.x, -12);
    EXPECT_EQ(c1.y, 0);

    c1 = Polar(14, Compass::NE).asCoord();
    EXPECT_EQ(c1.x, 10);
    EXPECT_EQ(c1.y, 10);

    c1 = Polar(-14, Compass::NE).asCoord();
    EXPECT_EQ(c1.x, -10);
    EXPECT_EQ(c1.y, -10);

    c1 = Polar(14, Compass::E).asCoord();
    EXPECT_EQ(c1.x, 14);
    EXPECT_EQ(c1.y, 0);

    c1 = Polar(-14, Compass::E).asCoord();
    EXPECT_EQ(c1.x, -14);
    EXPECT_EQ(c1.y, 0);

}

}
#include "../src/basicTypes.h"
#include <gtest/gtest.h>


namespace BS {

bool areClosef(float a, float b)
{
    return std::abs(a - b) < 0.0001;
}


TEST(CoordTests, TestDefaultCoord) {
    
    Coord c1 = Coord();
    EXPECT_EQ(0, c1.x);
    EXPECT_EQ(0, c1.y);
}

TEST(CoordTests, TestCoordConstructor) {
          
    Coord c2 = Coord(-6, 12);
    EXPECT_EQ(-6, c2.x);
    EXPECT_EQ(12, c2.y);
}

TEST(CoordTests, TestCoordCopyAssignment) {

    Coord c2 = Coord(9, 101);
    EXPECT_EQ(9, c2.x);
    EXPECT_EQ(101, c2.y);
}

TEST(CoordTests, TestCoordIsNormalized) {
    Coord c1 = Coord(9, 101);
    
    EXPECT_FALSE(c1.isNormalized());

    Coord c2 = Coord(1,1);
    EXPECT_TRUE(c2.isNormalized());
}

TEST(CoordTests, TestCoordNormalize) {
    Coord c1 = Coord(0, 0);
    Coord c2 = c1.normalize();

    EXPECT_EQ(0, c2.x);
    EXPECT_EQ(0, c2.y);

    EXPECT_EQ(Dir(Compass::CENTER), c2.asDir());

    Coord c3 = Coord(-500, 10).normalize();

    EXPECT_EQ(-1, c3.x);
    EXPECT_EQ(0, c3.y);
    EXPECT_EQ(Dir(Compass::W), c3.asDir());
}
TEST(CoordTests, TestCoordLength) {

    EXPECT_EQ(Coord(0, 0).length(), 0);
    EXPECT_EQ(Coord(0, 1).length(), 1);
    EXPECT_EQ(Coord(-1, 0).length(), 1);
    EXPECT_EQ(Coord(-1, -1).length(), 1); // round down
    EXPECT_EQ(Coord(22, 0).length(), 22);
    EXPECT_EQ(Coord(22, 22).length(), 31); // round down
    EXPECT_EQ(Coord(10, -10).length(), 14); // round down
    EXPECT_EQ(Coord(-310, 0).length(), 310);
}

TEST(CoordTests, TestCoordAsPolar) {
    Polar p1 = Coord(0, 0).asPolar();
    EXPECT_EQ(p1.mag, 0);
    EXPECT_EQ(Dir(Compass::CENTER), p1.dir);

    p1 = Coord(0, 1).asPolar();
    EXPECT_EQ(p1.mag, 1);
    EXPECT_EQ(Dir(Compass::N), p1.dir);

    p1 = Coord(-10, -10).asPolar();
    EXPECT_EQ(p1.mag, 14);
    EXPECT_EQ(Dir(Compass::SW), p1.dir); // round down mag

    p1 = Coord(100, 1).asPolar();
    EXPECT_EQ(p1.mag, 100);
    EXPECT_EQ(Dir(Compass::E), p1.dir); // round down mag
}

TEST(CoordTests, TestCoordPlus) {
    Coord c1 = Coord(0, 0) + Coord(6, 8);
    EXPECT_EQ(c1.x, 6);
    EXPECT_EQ(c1.y, 8);

    c1 = Coord(-70, 20) + Coord(10, -10);
    EXPECT_EQ(c1.x, -60);
    EXPECT_EQ(c1.y, 10);
}

TEST(CoordTests, TestCoordMinus) {

    Coord c1 = Coord(-70, 20) - Coord(10, -10);
    EXPECT_EQ(c1.x, -80);
    EXPECT_EQ(c1.y, 30);
}

TEST(CoordTests, TestCoordMultiply) {

    Coord c1 = Coord(0, 0) * 1;
    EXPECT_EQ(c1.x, 0);
    EXPECT_EQ(c1.y, 0);

    c1 = Coord(1, 1) * -5;
    EXPECT_EQ(c1.x, -5);
    EXPECT_EQ(c1.y, -5);

    c1 = Coord(11, 5) * -5;
    EXPECT_EQ(c1.x, -55);
    EXPECT_EQ(c1.y, -25);
}

TEST(CoordTests, TestCoordPlusDir) {
    Coord c1 = Coord(0, 0);
    Coord c2 = c1 + Dir(Compass::CENTER);
    EXPECT_EQ(c2.x, 0);
    EXPECT_EQ(c2.y, 0);

    c2 = c1 + Dir(Compass::E);
    EXPECT_EQ(c2.x, 1);
    EXPECT_EQ(c2.y, 0);
    
    c2 = c1 + Dir(Compass::W);
    EXPECT_EQ(c2.x, -1);
    EXPECT_EQ(c2.y, 0);
    
    c2 = c1 + Dir(Compass::SW);
    EXPECT_EQ(c2.x, -1);
    EXPECT_EQ(c2.y, -1);
}

TEST(CoordTests, TestCoordMinusDir) {
    Coord c1 = Coord(0, 0);
    Coord c2 = c1 - Dir(Compass::CENTER);
    EXPECT_EQ(c2.x, 0);
    EXPECT_EQ(c2.y, 0);

    c2 = c1 - Dir(Compass::E);
    EXPECT_EQ(c2.x, -1);
    EXPECT_EQ(c2.y, 0);

    c2 = c1 - Dir(Compass::W);
    EXPECT_EQ(c2.x, 1);
    EXPECT_EQ(c2.y, 0);

    c2 = c1 - Dir(Compass::SW);
    EXPECT_EQ(c2.x, 1);
    EXPECT_EQ(c2.y, 1);
}

TEST(CoordTests, TestRaySameness) {

    Coord c1 = Coord { 0, 0 };
    Coord c2 = Coord { 10, 11 };
    Dir d1 = Compass::CENTER;

    EXPECT_EQ(c1.raySameness(c2), 1.0); // special case - zero vector
    EXPECT_EQ(c2.raySameness(c1), 1.0); // special case - zero vector
    EXPECT_EQ(c2.raySameness(d1), 1.0); // special case - zero vector
}

TEST(CoordTests, TestCoordRaySelfSameness) {

    Coord c2 = Coord { 10, 11 };
    Coord c1 = c2;
    EXPECT_EQ(c1.raySameness(c2), 1.0);
    EXPECT_TRUE(areClosef(Coord(-10,-10).raySameness(Coord(10,10)), -1.0));
}

TEST(CoordTests, TestAreCloseF) {

    Coord c1 = Coord{0,11};
    Coord c2 = Coord{20,0};
    EXPECT_TRUE(areClosef(c1.raySameness(c2), 0.0));
    EXPECT_TRUE(areClosef(c2.raySameness(c1), 0.0));

    c1 = Coord{0,444};
    c2 = Coord{113,113};
    EXPECT_TRUE(areClosef(c1.raySameness(c2), 0.707106781));

    c2 = Coord{113,-113};
    EXPECT_TRUE(areClosef(c1.raySameness(c2), -0.707106781));
}
} 
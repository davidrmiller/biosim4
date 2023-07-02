#include "../src/basicTypes.h"
#include <gtest/gtest.h>


namespace BS {

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
} 
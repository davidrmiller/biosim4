// #include "../src/common/basicTypes.h"
#include "../src/grid.h"
#include <gtest/gtest.h>

namespace BS {

TEST(GridTests, TestGridSizes) {
    
    Grid g = Grid();
    uint16_t sizeX = 128;
    uint16_t sizeY = 128;
    g.init(sizeX, sizeY);

    EXPECT_EQ(sizeX, g.sizeX());
    EXPECT_EQ(sizeY, g.sizeY());

}

TEST(GridTests, TestGridIsInBounds) {
    
    Grid g = Grid();
    uint16_t sizeX = 128;
    uint16_t sizeY = 128;
    g.init(sizeX, sizeY);

    Coord c1 = Coord(0, 0);

    EXPECT_TRUE(g.isInBounds(c1));

    Coord c2 = Coord(sizeX, sizeY);

    EXPECT_FALSE(g.isInBounds(c2));

    Coord c3 = Coord(sizeX, 0);

    EXPECT_FALSE(g.isInBounds(c3));

    Coord c4 = Coord(0, sizeY);

    EXPECT_FALSE(g.isInBounds(c4));

    Coord c5 = Coord(sizeX/2, sizeY/2);

    EXPECT_TRUE(g.isInBounds(c5));
}

}
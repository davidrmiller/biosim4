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

TEST(GridTests, TestGridIsEmptyAt) {
    Grid g = Grid();
    uint16_t sizeX = 128;
    uint16_t sizeY = 128;
    g.init(sizeX, sizeY);

    Coord c1 = Coord(0, 0);
    EXPECT_TRUE(g.isEmptyAt(c1));

    Coord c2 = Coord(10, 10);
    g.set(c2, BARRIER);
    EXPECT_FALSE(g.isEmptyAt(c2));

    Coord c3 = Coord(100, 100);
    g.set(c3, 50);
    EXPECT_FALSE(g.isEmptyAt(c3));
}

TEST(GridTests, TestGridIsBarrierAt) {
    Grid g = Grid();
    uint16_t sizeX = 128;
    uint16_t sizeY = 128;
    g.init(sizeX, sizeY);

    Coord c1 = Coord(30, 70);
    EXPECT_FALSE(g.isBarrierAt(c1));

    Coord c2 = Coord(10, 10);
    g.set(c2, BARRIER);
    EXPECT_TRUE(g.isBarrierAt(c2));
}

TEST(GridTests, TestGridIsOccupiedAt) {
    Grid g = Grid();
    uint16_t sizeX = 128;
    uint16_t sizeY = 128;
    g.init(sizeX, sizeY);

    Coord c1 = Coord(100, 80);
    EXPECT_FALSE(g.isOccupiedAt(c1));

    Coord c2 = Coord(10, 10);
    g.set(c2, 55);
    EXPECT_TRUE(g.isOccupiedAt(c2));

    Coord c3 = Coord(10, 10);
    g.set(c3, BARRIER);
    EXPECT_FALSE(g.isOccupiedAt(c2));
}

}
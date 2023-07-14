#include "../../grid.h"

#include <catch2/catch_test_macros.hpp>

namespace BS {

TEST_CASE("TestGridSizes","[Grid]") {
    
    Grid g = Grid();
    uint16_t sizeX = 128;
    uint16_t sizeY = 128;
    g.init(sizeX, sizeY);

    CHECK(sizeX == g.sizeX());
    CHECK(sizeY == g.sizeY());

}

TEST_CASE("TestGridIsInBounds","[Grid]") {
    
    Grid g = Grid();
    uint16_t sizeX = 128;
    uint16_t sizeY = 128;
    g.init(sizeX, sizeY);

    Coord c1 = Coord(0, 0);

    CHECK(g.isInBounds(c1));

    Coord c2 = Coord(sizeX, sizeY);

    CHECK_FALSE(g.isInBounds(c2));

    Coord c3 = Coord(sizeX, 0);

    CHECK_FALSE(g.isInBounds(c3));

    Coord c4 = Coord(0, sizeY);

    CHECK_FALSE(g.isInBounds(c4));

    Coord c5 = Coord(sizeX/2, sizeY/2);

    CHECK(g.isInBounds(c5));
}

TEST_CASE("TestGridIsEmptyAt","[Grid]") {
    Grid g = Grid();
    uint16_t sizeX = 128;
    uint16_t sizeY = 128;
    g.init(sizeX, sizeY);

    Coord c1 = Coord(0, 0);
    CHECK(g.isEmptyAt(c1));

    Coord c2 = Coord(10, 10);
    g.set(c2, BARRIER);
    CHECK_FALSE(g.isEmptyAt(c2));

    Coord c3 = Coord(100, 100);
    g.set(c3, 50);
    CHECK_FALSE(g.isEmptyAt(c3));
}

TEST_CASE("TestGridIsBarrierAt","[Grid]") {
    Grid g = Grid();
    uint16_t sizeX = 128;
    uint16_t sizeY = 128;
    g.init(sizeX, sizeY);

    Coord c1 = Coord(30, 70);
    CHECK_FALSE(g.isBarrierAt(c1));

    Coord c2 = Coord(10, 10);
    g.set(c2, BARRIER);
    CHECK(g.isBarrierAt(c2));
}

TEST_CASE("TestGridIsOccupiedAt","[Grid]") {
    Grid g = Grid();
    uint16_t sizeX = 128;
    uint16_t sizeY = 128;
    g.init(sizeX, sizeY);

    Coord c1 = Coord(100, 80);
    CHECK_FALSE(g.isOccupiedAt(c1));

    Coord c2 = Coord(10, 10);
    g.set(c2, 55);
    CHECK(g.isOccupiedAt(c2));

    Coord c3 = Coord(10, 10);
    g.set(c3, BARRIER);
    CHECK_FALSE(g.isOccupiedAt(c3));
}

TEST_CASE("TestGridIsBorder","[Grid]") {
    Grid g = Grid();
    uint16_t sizeX = 128;
    uint16_t sizeY = 128;
    g.init(sizeX, sizeY);

    Coord c1 = Coord(0, 0);
    CHECK(g.isBorder(c1));

    Coord c2 = Coord(10, 10);
    CHECK_FALSE(g.isBorder(c2));

    Coord c3 = Coord(sizeX-1, 10);
    CHECK(g.isBorder(c3));
}

TEST_CASE("TestSetBarrier","[Grid]") {

    Grid g = Grid();
    uint16_t sizeX = 128;
    uint16_t sizeY = 128;
    g.init(sizeX, sizeY);

    Coord c1 = Coord(10, 10);

    CHECK_FALSE(g.isBarrierAt(c1));

    g.setBarrier(c1);

    CHECK(g.isBarrierAt(c1));

}


}
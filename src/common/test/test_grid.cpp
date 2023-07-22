#include "../include/grid.h"
#include "../include/dir.h"
#include "../include/compass.h"
#include "../include/coord.h"
// #include "../include/polar.h"

// #include "../../createGridBarrierVisitor.h"

#include <catch2/catch_test_macros.hpp>

namespace BS {

    TEST_CASE("TestGridSizes","[Grid]") {
        
        Grid g = Grid();
        uint16_t numLayers = 1;
        uint16_t sizeX = 128;
        uint16_t sizeY = 128;
        g.init(numLayers, sizeX, sizeY);

        CHECK(sizeX == g.sizeX());
        CHECK(sizeY == g.sizeY());

    }

TEST_CASE("TestDefaultGridIsEmpty","[Grid]") {
    
    Grid g = Grid();

    CHECK(0 == g.sizeX());
    CHECK(0 == g.sizeY());

}

TEST_CASE("TestGridIsInBounds","[Grid]") {
    
    Grid g = Grid();
    uint16_t numLayers = 1;
    uint16_t sizeX = 128;
    uint16_t sizeY = 128;
    g.init(numLayers, sizeX, sizeY);

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
        uint16_t numLayers = 1;
        uint16_t sizeX = 128;
        uint16_t sizeY = 128;
        g.init(numLayers, sizeX, sizeY);

        Coord c1 = Coord(0, 0);
        CHECK(g.isEmptyAt(c1));

        Coord c2 = Coord(10, 10);
        g.set(c2, Grid::BARRIER);
        CHECK_FALSE(g.isEmptyAt(c2));

        Coord c3 = Coord(100, 100);
        g.set(c3, 50);
        CHECK_FALSE(g.isEmptyAt(c3));
    }

    TEST_CASE("TestGridIsBarrierAt","[Grid]") {
        Grid g = Grid();
        uint16_t numLayers = 1;
        uint16_t sizeX = 128;
        uint16_t sizeY = 128;
        g.init(numLayers, sizeX, sizeY);

        Coord c1 = Coord(30, 70);
        CHECK_FALSE(g.isBarrierAt(c1));

        Coord c2 = Coord(10, 10);
        g.set(c2, Grid::BARRIER);
        CHECK(g.isBarrierAt(c2));
    }

    TEST_CASE("TestGridIsOccupiedAt","[Grid]") {
        Grid g = Grid();
        uint16_t numLayers = 1;
        uint16_t sizeX = 128;
        uint16_t sizeY = 128;
        g.init(numLayers, sizeX, sizeY);

        Coord c1 = Coord(100, 80);
        CHECK_FALSE(g.isOccupiedAt(c1));

        Coord c2 = Coord(10, 10);
        g.set(c2, 55);
        CHECK(g.isOccupiedAt(c2));

        Coord c3 = Coord(10, 10);
        g.set(c3, Grid::BARRIER);
        CHECK_FALSE(g.isOccupiedAt(c3));
    }

    TEST_CASE("TestGridIsBorder","[Grid]") {
        Grid g = Grid();
        uint16_t numLayers = 1;
        uint16_t sizeX = 128;
        uint16_t sizeY = 128;
        g.init(numLayers, sizeX, sizeY);
        g.init(numLayers, sizeX, sizeY);

        Coord c1 = Coord(0, 0);
        CHECK(g.isBorder(c1));

        Coord c2 = Coord(10, 10);
        CHECK_FALSE(g.isBorder(c2));

        Coord c3 = Coord(sizeX-1, 10);
        CHECK(g.isBorder(c3));
    }

    TEST_CASE("TestSetBarrier","[Grid]") {

        Grid g = Grid();
        uint16_t numLayers = 1;
        uint16_t sizeX = 128;
        uint16_t sizeY = 128;
        g.init(numLayers, sizeX, sizeY);

        Coord c1 = Coord(10, 10);

        CHECK_FALSE(g.isBarrierAt(c1));

        CHECK(g.setBarrier(c1));
        g.setBarrier(c1);

        CHECK(g.isBarrierAt(c1));

    }

    TEST_CASE("TestSetBarrierFailsWhenOutOfBounds","[Grid]") {

        Grid g = Grid();
        uint16_t numLayers = 1;
        uint16_t sizeX = 128;
        uint16_t sizeY = 128;
        g.init(numLayers, sizeX, sizeY);

        Coord c1 = Coord(200, 200);

        CHECK_FALSE(g.setBarrier(c1));

        CHECK_FALSE(g.isBarrierAt(c1));

    }

    TEST_CASE("TestZeroFillClearsBarriers","[Grid]") {

        Grid g = Grid();
        uint16_t numLayers = 1;
        uint16_t sizeX = 128;
        uint16_t sizeY = 128;
        g.init(numLayers, sizeX, sizeY);

        Coord c1 = Coord(10, 10);
        g.setBarrier(c1);
        CHECK(g.isBarrierAt(c1));
        CHECK_FALSE(g.getBarrierLocations().empty());
        CHECK(g.getBarrierLocations().size() == 1);

        g.zeroFill();

        CHECK_FALSE(g.isBarrierAt(c1));
        // test barrier locations are empty
        CHECK(g.getBarrierLocations().empty());
    }

    TEST_CASE("TestGetBarrierLocations","[Grid]") {

        Grid g = Grid();
        uint16_t numLayers = 1;
        uint16_t sizeX = 128;
        uint16_t sizeY = 128;
        g.init(numLayers, sizeX, sizeY);

        CHECK(g.getBarrierLocations().empty());

        g.setBarrier({10,20});

        CHECK(g.getBarrierLocations().size() == 1);
    }

    TEST_CASE("TestLongProbeBarrierFwd","[Grid]") {

        Grid g = Grid();
        uint16_t numLayers = 1;
        uint16_t sizeX = 128;
        uint16_t sizeY = 128;
        g.init(numLayers, sizeX, sizeY);

        std::vector<Dir> directions = {Compass::N, Compass::NE, Compass::NW, Compass::W,
        Compass::SW, Compass::S, Compass::SE, Compass::E};

        SECTION("With no barriers") {
            
            Coord loc = {0,0};

            unsigned longProbeDist = 200;
            
            for (Dir dir : directions){
                unsigned result = g.longProbeBarrierFwd(loc, dir, longProbeDist);
                CHECK(result == longProbeDist);
            }
        }
    }
        // SECTION("With vertical barrier") {
        //     Coord loc = {64,64};
        //     unsigned longProbeDist = 10;

    TEST_CASE("TestGetLayerMagnitudeWithZeroLayers","[Grid]") {

        Grid g = Grid();
        uint16_t numLayers = 0;
        uint16_t sizeX = 128;
        uint16_t sizeY = 128;
        g.init(numLayers, sizeX, sizeY);

        Coord c1 = Coord(10, 10);
        uint8_t mag = g.getLayerMagnitude(1, c1);
        CHECK(0 == mag);
    }

    TEST_CASE("TestGetLayerMagnitude","[Grid]") {

        Grid g = Grid();
        uint16_t numLayers = 1;
        uint16_t sizeX = 128;
        uint16_t sizeY = 128;
        g.init(numLayers, sizeX, sizeY);

        Coord c1 = Coord(10, 10);
        uint8_t mag = g.getLayerMagnitude(0, c1);
        CHECK(0 == mag);
    }

    TEST_CASE("TestFadeLayerWithZeroLayers","[Grid]") {

        Grid g = Grid();
        uint16_t numLayers = 0;
        uint16_t sizeX = 128;
        uint16_t sizeY = 128;
        g.init(numLayers, sizeX, sizeY);

        g.fadeLayer(1);

        Coord c1 = Coord(10, 10);
        uint8_t mag = g.getLayerMagnitude(1, c1);
        CHECK(0 == mag);
    }

    TEST_CASE("TestFadeLayer","[Grid]") {

        Grid g = Grid();
        uint16_t numLayers = 1;
        uint16_t sizeX = 128;
        uint16_t sizeY = 128;
        g.init(numLayers, sizeX, sizeY);

        g.fadeLayer(1);

        Coord c1 = Coord(10, 10);
        uint8_t mag = g.getLayerMagnitude(0, c1);
        CHECK(0 == mag);
    }

    TEST_CASE("TestHasLayerZeroLayers","[Grid]") {

        Grid g = Grid();
        uint16_t numLayers = 0;
        uint16_t sizeX = 128;
        uint16_t sizeY = 128;
        g.init(numLayers, sizeX, sizeY);

        CHECK_FALSE(g.hasLayer(0));
        CHECK_FALSE(g.hasLayer(1));
        
        CHECK_FALSE(g.hasLayer(10));
    }

    TEST_CASE("TestHasLayer","[Grid]") {

        Grid g = Grid();
        uint16_t numLayers = 1;
        uint16_t sizeX = 128;
        uint16_t sizeY = 128;
        g.init(numLayers, sizeX, sizeY);

        CHECK(g.hasLayer(0));
        CHECK_FALSE(g.hasLayer(1));
        
        CHECK_FALSE(g.hasLayer(10));
    }
}
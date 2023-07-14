#include "../include/polar.h"
#include "../include/dir.h"
#include "../include/compass.h"
#include "../include/coord.h"

#include <catch2/catch_test_macros.hpp>

namespace BS {

TEST_CASE("TestPolarDefault","[Polar]") {
    
    Polar p1 = Polar();

    CHECK(p1.mag == 0);
    CHECK(p1.dir == Dir(Compass::CENTER));

}

TEST_CASE("TestPolarConstruct","[Polar]") {
    Polar p1 = Polar(-10, Compass::NW);
    CHECK(p1.mag == -10);
    CHECK(p1.dir == Dir(Compass::NW));
}

TEST_CASE("TestPolarAsCoord","[Polar]") {

    Coord c1 = Polar(0, Compass::CENTER).asCoord();
    CHECK(c1.x == 0);
    CHECK(c1.y == 0);

    c1 = Polar(10, Compass::CENTER).asCoord();
    CHECK(c1.x == 0);
    CHECK(c1.y == 0);
    
    c1 = Polar(20, Compass::N).asCoord();
    CHECK(c1.x == 0);
    CHECK(c1.y == 20);

    Polar p1 = Polar(12, Compass::W);
    c1 = p1.asCoord();
    CHECK(c1.x == -12);
    CHECK(c1.y == 0);

    c1 = Polar(14, Compass::NE).asCoord();
    CHECK(c1.x == 10);
    CHECK(c1.y == 10);

    c1 = Polar(-14, Compass::NE).asCoord();
    CHECK(c1.x == -10);
    CHECK(c1.y == -10);

    c1 = Polar(14, Compass::E).asCoord();
    CHECK(c1.x == 14);
    CHECK(c1.y == 0);

    c1 = Polar(-14, Compass::E).asCoord();
    CHECK(c1.x == -14);
    CHECK(c1.y == 0);

}

}
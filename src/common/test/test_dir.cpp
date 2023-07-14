#include "../include/dir.h"
#include "../include/compass.h"
#include "../include/coord.h"
#include "../include/polar.h"

#include <catch2/catch_test_macros.hpp>

namespace BS {

TEST_CASE("TestDirAsInt","[Dir]") {
    
    Dir d1 = Dir(Compass::N);

    CHECK(d1.asInt() == (int)Compass::N);

    CHECK_FALSE(d1.asInt() == 0);
}

TEST_CASE("TestDirCopyAssignment","[Dir]") {
    
    Dir d1 = Dir(Compass::N);
    Dir d2 = Dir(Compass::CENTER);

    d1 = d2;

    CHECK(d1.asInt() == (int)Compass::CENTER);

    CHECK_FALSE(d1.asInt() == 0);
}

TEST_CASE("TestDirEquality","[Dir]") {

    CHECK(Dir(Compass::W) == Compass::W);

    Dir d3 = Compass::N;
    Dir d4 = Compass::N;
    CHECK(d3 == d4);

    Dir d5 = Compass::NE;
    CHECK_FALSE(d3 == d5);
}

TEST_CASE("TestDirRotate","[Dir]") {

    Dir d1 = Compass::NE;
    // rotation happens in 45 degree increments, positive is clockwise
    CHECK(d1.rotate(1) == Compass::E);
    CHECK(d1.rotate(2) == Compass::SE);
    CHECK(d1.rotate(-1) == Compass::N);
    CHECK(d1.rotate(-2) == Compass::NW);
    CHECK(Dir(Compass::N).rotate(1) == d1);
    CHECK(Dir(Compass::SW).rotate(-2) == Compass::SE);
}

TEST_CASE("TestDirAsNormalisedCoord","[Dir]") {

    Coord c1 = Dir(Compass::CENTER).asNormalizedCoord();

    CHECK(0 == c1.x);
    CHECK(0 == c1.y);

    Dir d1 = Compass::SW;
    
    Coord c2 = d1.asNormalizedCoord();
    CHECK(-1 == c2.x);
    CHECK(-1 == c2.y);

}

TEST_CASE("TestDirAsNormalizedPolar","[Dir]") {

    Dir d1 = Compass::SW;
    Polar p1 = d1.asNormalizedPolar();
    
    CHECK(1 == p1.mag);
    CHECK(d1 == p1.dir);

    Dir d2 = Compass::S;
    Polar p2 = d2.asNormalizedPolar();

    CHECK(1 == p2.mag);
    CHECK(d2 == p2.dir);

}

} // namespace BS
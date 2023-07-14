#include "../include/coord.h"
#include "../include/compass.h"
#include "../include/dir.h"
#include "../include/polar.h"

#include <catch2/catch_test_macros.hpp>

namespace BS {

bool areClosef(float a, float b)
{
    return std::abs(a - b) < 0.0001;
}


TEST_CASE("TestDefaultCoord","[Coord]") {
    
    Coord c1 = Coord();
    CHECK(0 == c1.x);
    CHECK(0 == c1.y);
}

TEST_CASE("TestCoordConstructor","[Coord]") {
          
    Coord c2 = Coord(-6, 12);
    CHECK(-6 == c2.x);
    CHECK(12 == c2.y);
}

TEST_CASE("TestCoordCopyAssignment","[Coord]") {

    Coord c2 = Coord(9, 101);
    CHECK(9 == c2.x);
    CHECK(101 == c2.y);
}

TEST_CASE("TestCoordIsNormalized","[Coord]") {
    Coord c1 = Coord(9, 101);
    
    CHECK_FALSE(c1.isNormalized());

    Coord c2 = Coord(1,1);
    CHECK(c2.isNormalized());
}

TEST_CASE("TestCoordNormalize","[Coord]") {
    Coord c1 = Coord(0, 0);
    Coord c2 = c1.normalize();

    CHECK(0 == c2.x);
    CHECK(0 == c2.y);

    CHECK(Dir(Compass::CENTER) == c2.asDir());

    Coord c3 = Coord(-500, 10).normalize();

    CHECK(-1 == c3.x);
    CHECK(0 == c3.y);
    CHECK(Dir(Compass::W) == c3.asDir());
}
TEST_CASE("TestCoordLength","[Coord]") {

    CHECK(Coord(0, 0).length() == 0);
    CHECK(Coord(0, 1).length() == 1);
    CHECK(Coord(-1, 0).length() == 1);
    CHECK(Coord(-1, -1).length() == 1); // round down
    CHECK(Coord(22, 0).length() == 22);
    CHECK(Coord(22, 22).length() == 31); // round down
    CHECK(Coord(10, -10).length() == 14); // round down
    CHECK(Coord(-310, 0).length() == 310);
}

TEST_CASE("TestCoordAsPolar","[Coord]") {
    Polar p1 = Coord(0, 0).asPolar();
    CHECK(p1.mag == 0);
    CHECK(Dir(Compass::CENTER) == p1.dir);

    p1 = Coord(0, 1).asPolar();
    CHECK(p1.mag == 1);
    CHECK(Dir(Compass::N) == p1.dir);

    p1 = Coord(-10, -10).asPolar();
    CHECK(p1.mag == 14);
    CHECK(Dir(Compass::SW) == p1.dir); // round down mag

    p1 = Coord(100, 1).asPolar();
    CHECK(p1.mag == 100);
    CHECK(Dir(Compass::E) == p1.dir); // round down mag
}

TEST_CASE("TestCoordPlus","[Coord]") {
    Coord c1 = Coord(0, 0) + Coord(6, 8);
    CHECK(c1.x == 6);
    CHECK(c1.y == 8);

    c1 = Coord(-70, 20) + Coord(10, -10);
    CHECK(c1.x == -60);
    CHECK(c1.y == 10);
}

TEST_CASE("TestCoordMinus","[Coord]") {

    Coord c1 = Coord(-70, 20) - Coord(10, -10);
    CHECK(c1.x == -80);
    CHECK(c1.y == 30);
}

TEST_CASE("TestCoordMultiply","[Coord]") {

    Coord c1 = Coord(0, 0) * 1;
    CHECK(c1.x == 0);
    CHECK(c1.y == 0);

    c1 = Coord(1, 1) * -5;
    CHECK(c1.x == -5);
    CHECK(c1.y == -5);

    c1 = Coord(11, 5) * -5;
    CHECK(c1.x == -55);
    CHECK(c1.y == -25);
}

TEST_CASE("TestCoordPlusDir","[Coord]") {
    Coord c1 = Coord(0, 0);
    Coord c2 = c1 + Dir(Compass::CENTER);
    CHECK(c2.x == 0);
    CHECK(c2.y == 0);

    c2 = c1 + Dir(Compass::E);
    CHECK(c2.x == 1);
    CHECK(c2.y == 0);
    
    c2 = c1 + Dir(Compass::W);
    CHECK(c2.x == -1);
    CHECK(c2.y == 0);
    
    c2 = c1 + Dir(Compass::SW);
    CHECK(c2.x == -1);
    CHECK(c2.y == -1);
}

TEST_CASE("TestCoordMinusDir","[Coord]") {
    Coord c1 = Coord(0, 0);
    Coord c2 = c1 - Dir(Compass::CENTER);
    CHECK(c2.x == 0);
    CHECK(c2.y == 0);

    c2 = c1 - Dir(Compass::E);
    CHECK(c2.x == -1);
    CHECK(c2.y == 0);

    c2 = c1 - Dir(Compass::W);
    CHECK(c2.x == 1);
    CHECK(c2.y == 0);

    c2 = c1 - Dir(Compass::SW);
    CHECK(c2.x == 1);
    CHECK(c2.y == 1);
}

TEST_CASE("TestRaySameness","[Coord]") {

    Coord c1 = Coord { 0, 0 };
    Coord c2 = Coord { 10, 11 };
    Dir d1 = Compass::CENTER;

    CHECK(c1.raySameness(c2) == 1.0); // special case - zero vector
    CHECK(c2.raySameness(c1) == 1.0); // special case - zero vector
    CHECK(c2.raySameness(d1) == 1.0); // special case - zero vector
}

TEST_CASE("TestCoordRaySelfSameness","[Coord]") {

    Coord c2 = Coord { 10, 11 };
    Coord c1 = c2;
    CHECK(c1.raySameness(c2) == 1.0);
    CHECK(areClosef(Coord(-10,-10).raySameness(Coord(10,10)), -1.0));
}

TEST_CASE("TestAreCloseF","[Coord]") {

    Coord c1 = Coord{0,11};
    Coord c2 = Coord{20,0};
    CHECK(areClosef(c1.raySameness(c2), 0.0));
    CHECK(areClosef(c2.raySameness(c1), 0.0));

    c1 = Coord{0,444};
    c2 = Coord{113,113};
    CHECK(areClosef(c1.raySameness(c2), 0.707106781));

    c2 = Coord{113,-113};
    CHECK(areClosef(c1.raySameness(c2), -0.707106781));
}
} 
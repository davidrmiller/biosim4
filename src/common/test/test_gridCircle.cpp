#include "../include/gridCircle.h"
#include "../include/coord.h"
#include "../include/grid.h"

#include <catch2/catch_test_macros.hpp>


namespace BS {

    TEST_CASE("TestGridCircle","[GridCircle]") {
        
        Grid g = Grid();
        Coord loc = Coord();
        float radius = 10;

        GridCircle c = GridCircle(g, loc, radius);

    }

}
#include "../include/compass.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("TestCompass", "[Compass]") {
    
    BS::Compass compass = BS::Compass(BS::Compass::SW);

    CHECK(0 == (uint8_t)compass);
}

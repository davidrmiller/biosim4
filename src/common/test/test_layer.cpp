#include "../include/layer.h"

#include <catch2/catch_test_macros.hpp>

namespace BS {

    TEST_CASE("TestLayerZeroFill", "[Layer]") {
        
        Layer l = Layer(32, 32);
        l.zeroFill();

        
        CHECK(l[0][0] == 0);
        CHECK(l[31][31] == 0);
        CHECK(l[0][15] == 0);
        CHECK(l[15][0] == 0);

    }

    TEST_CASE("TestSetLayerCell", "[Layer]") {
        
        Layer l = Layer(32, 32);
        l.zeroFill();

        uint16_t val = 12345;

        l[0][0] = val;

        CHECK(l[0][0] == val);

    }

}
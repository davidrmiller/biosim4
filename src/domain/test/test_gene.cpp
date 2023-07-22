#include "../include/gene.h"
#include <catch2/catch_test_macros.hpp>

namespace BS {

    TEST_CASE("TestWeight","[Gene]") {
        
        Gene g;

        g.weight = 2;

        CHECK(g.weight == 2);
    }

    TEST_CASE("TestSourceType","[Gene]") {
        
        Gene g;
        g.sourceType = NEURON;

        CHECK(g.sourceType == NEURON);
    }
}
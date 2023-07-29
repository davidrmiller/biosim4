#include "../include/gene.h"
#include "../../random.h"
#include <catch2/catch_test_macros.hpp>

namespace BS {

    TEST_CASE("TestWeight","[Gene]") {
        
        Gene g;

        g.weight = 2;

        CHECK(g.weight == 2);
    }

    /**
     * SENSOR = 1;  // always a source
     * ACTION = 1;  // always a sink
     * NEURON = 0;  // can be either a source or sink
    */
    TEST_CASE("TestSourceType","[Gene]") {
        
        Gene g;
        g.sourceType = NEURON;

        CHECK(g.sourceType == NEURON);

        g.sourceType = randomUint() & 1;
        g.sinkType = randomUint() & 1;

        CHECK(((g.sourceType == NEURON) || (g.sourceType == SENSOR))) ;
        CHECK(((g.sinkType == NEURON) || (g.sinkType == ACTION))) ;
        CHECK_FALSE(g.sourceType == ACTION);
        CHECK_FALSE(g.sinkType == SENSOR);
    }

    TEST_CASE("TestEquality","[Gene]") {
        
        Gene g;
        g.sourceType = NEURON;
        g.sourceNum = 1;
        g.sinkType = ACTION;
        g.sinkNum = 5;
        g.weight = 56;

        Gene g2;
        g2.sourceType = NEURON;
        g2.sourceNum = 1;
        g2.sinkType = ACTION;
        g2.sinkNum = 5;
        g2.weight = 56;

        bool result = false;

        SECTION("Equals") {
            result = (g == g2);
            CHECK(result);
        }

        SECTION("Different sourceNum") {
            g2.sourceNum = 10;
            result = (g != g2);

            CHECK(result); 
        }

        SECTION("Different sourceType") {
            g2.sourceType = SENSOR;
            result = (g != g2);

            CHECK(result); 
        }

        SECTION("Different sinkType") {
            g2.sinkType = NEURON;
            result = (g != g2);

            CHECK(result); 
        }

        SECTION("Different sinkNum") {
            g2.sinkNum = 33;
            result = (g != g2);

            CHECK(result); 
        }

        SECTION("Different weight") {
            g2.weight = 101;
            result = (g != g2);

            CHECK(result); 
        }    
    }
}
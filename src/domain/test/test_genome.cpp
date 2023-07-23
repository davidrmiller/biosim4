#include "../include/genome.h"
#include "./test_helper.h"

#include <catch2/catch_test_macros.hpp>

namespace BS {

    TEST_CASE("TestSize","[Genome]") {
        
        Genome genome;
    
        CHECK(genome.size() == 0);
    }

    TEST_CASE("TestAdd","[Genome]") {
        
        Gene g = makeRandomGene();

        Genome genome;
        genome.add(g);

        CHECK(genome.size() == 1);
    }

}
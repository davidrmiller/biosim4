#include "../include/connectionList.h"
#include "../include/gene.h"
#include "./test_helper.h"

#include <catch2/catch_test_macros.hpp>

namespace BS {

    TEST_CASE("TestEmptyByDefault","[ConnectionList]") {
        
        unsigned maxNeurons = 5;
        NodeMap n;
        ConnectionList c = ConnectionList(maxNeurons, n);

        CHECK(c.connections().size() == 0);
    }

    TEST_CASE("TestRenumber","[ConnectionList]") {
        
        Genome genome;
        for (unsigned n = 0; n < 5; ++n) {
            genome.add(Gene());
        }

        unsigned maxNeurons = 5;
        NodeMap n;
        ConnectionList c = ConnectionList(maxNeurons, n);

        CHECK(c.connections().size() == 0);

        c.renumber(genome);

        CHECK(c.connections().size() == 5);

        // CHECK(n.size() == 5);
    }

    TEST_CASE("TestMakeNodeList","[ConnectionList]") {
        
        Genome genome;
        for (unsigned n = 0; n < 5; ++n) {
            genome.add(makeRandomGene());
        }

        unsigned maxNeurons = 5;
        NodeMap n;
        ConnectionList c = ConnectionList(maxNeurons, n);

        CHECK(c.connections().size() == 0);

        c.renumber(genome);
        c.makeNodeList();

        CHECK(c.connections().size() == 5);

        CHECK(n.size() > 0);
    }
}
#include "../include/neuralNet.h"
#include "../include/gene.h"
#include <catch2/catch_test_macros.hpp>

namespace BS {

    TEST_CASE("TestCreate","[NeuralNet]") {
        
        unsigned size = 10;
        NeuralNet nnet;

        nnet.neurons.clear();

        for (unsigned neuronNum = 0; neuronNum < size; ++neuronNum) {
            nnet.neurons.push_back( {} );
            nnet.neurons.back().output = 0.5;
            nnet.neurons.back().driven = true;
        } 

        CHECK(nnet.neurons.size() == size);
    }
    
    TEST_CASE("TestHasConnections","[NeuralNet]") {
        NeuralNet nnet;

        CHECK_FALSE(nnet.hasConnections());

        Gene g;
        nnet.addConnection(g);

        CHECK(nnet.hasConnections());
    }

    TEST_CASE("TestGetConnections","[NeuralNet]") {
        NeuralNet nnet;

        CHECK_FALSE(nnet.hasConnections());

        Gene g;
        nnet.addConnection(g);

        const std::vector<Gene> &connections = nnet.getConnections();

        CHECK(connections.size() == 1);
        
    }

}
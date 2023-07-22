#include "../include/neuralNet.h"
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
}
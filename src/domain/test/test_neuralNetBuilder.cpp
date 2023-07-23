#include "../include/neuralNetBuilder.h"
#include "./test_helper.h"

#include <catch2/catch_test_macros.hpp>

namespace BS {

    TEST_CASE("TestBuild","[NeuralNetBuilder]") {
        
        unsigned maxNeurons = 16;

        NeuralNetBuilder builder = NeuralNetBuilder(maxNeurons);

        std::shared_ptr<Genome> genome = makeRandomGenome(12, 16);

        std::shared_ptr<NeuralNet> nnet = builder.buildNeuralNet(genome);

        CHECK(nnet->connections.size() > 0);
    }
}
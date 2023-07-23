#pragma once
#include <memory>
#include <cassert>

#include "./neuralNet.h"
#include "./genome.h"
#include "./connectionList.h"

namespace BS
{
    constexpr float initialNeuronOutput() { return 0.5; }

    /**
     * Builds a NeuralNet from a Genome
     * Returns smart pointers to the NeuralNet
    */
    class NeuralNetBuilder {

        public:
        
            NeuralNetBuilder(unsigned maxNeurons);

            std::shared_ptr<NeuralNet> buildNeuralNet(std::shared_ptr<Genome> genome) const;
        
        private:
            unsigned maxNeurons;

    };

} // namespace BS

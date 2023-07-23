#pragma once
#include <memory>
#include "./neuralNet.h"
#include "./genome.h"

namespace BS
{
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

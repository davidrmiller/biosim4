#pragma once
#include <cstdint>
#include <vector>

namespace BS
{
    
    // Each gene specifies one synaptic connection in a neural net. Each
    // connection has an input (source) which is either a sensor or another neuron.
    // Each connection has an output, which is either an action or another neuron.
    // Each connection has a floating point weight derived from a signed 16-bit
    // value. The signed integer weight is scaled to a small range, then cubed
    // to provide fine resolution near zero.

    constexpr uint8_t SENSOR = 1;  // always a source
    constexpr uint8_t ACTION = 1;  // always a sink
    constexpr uint8_t NEURON = 0;  // can be either a source or sink

    class Gene {

        public:
            uint16_t sourceType:1; // SENSOR or NEURON
            uint16_t sourceNum:7;
            uint16_t sinkType:1;   // NEURON or ACTION
            uint16_t sinkNum:7;
            int16_t weight;

            static constexpr float f1 = 8.0;
            static constexpr float f2 = 64.0;

            float weightAsFloat() const;

            bool operator== (const Gene& g1) const;
            bool operator!= (const Gene& g1) const;

    };


    // An individual's genome is a set of Genes (see Gene comments above). Each
    // gene is equivalent to one connection in a neural net. An individual's
    // neural net is derived from its set of genes.

    // typedef std::vector<Gene> Genome;
    
} // namespace BS

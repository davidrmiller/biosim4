#pragma once
#include <cstdint>
#include <vector>

namespace BS
{
    

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
        // static int16_t makeRandomWeight() { return randomUint(0, 0xffff) - 0x8000; }
    };

    typedef std::vector<Gene> Genome;
    
} // namespace BS

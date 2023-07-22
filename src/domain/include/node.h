#pragma once

#include <cstdint>

namespace BS {
    // This structure is used while converting the connection list to a
    // neural net. This helps us to find neurons that don't feed anything
    // so that they can be removed along with all the connections that
    // feed the useless neurons. We'll cull neurons with .numOutputs == 0
    // or those that only feed themselves, i.e., .numSelfInputs == .numOutputs.
    // Finally, we'll renumber the remaining neurons sequentially starting
    // at zero using the .remappedNumber member.
    struct Node {
        uint16_t remappedNumber;
        uint16_t numOutputs;
        uint16_t numSelfInputs;
        uint16_t numInputsFromSensorsOrOtherNeurons;
    };
}
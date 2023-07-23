#pragma once
#include "../include/gene.h"

namespace BS {

    extern Gene makeRandomGene();
    extern Gene makeGene(uint16_t sourceType, uint16_t sourceNum, uint16_t sinkType, uint16_t sinkNum, uint16_t weight);

} // end namespace BS


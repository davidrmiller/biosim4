#include "./test_helper.h"
#include "../../random.h"

namespace BS
{
    
    Gene makeRandomGene()
    {
        Gene gene;

        gene.sourceType = randomUint() & 1;
        gene.sourceNum = (uint16_t)randomUint(0, 0x7fff);
        gene.sinkType = randomUint() & 1;
        gene.sinkNum = (uint16_t)randomUint(0, 0x7fff);
        gene.weight =  randomUint(0, 0xffff) - 0x8000;


        return gene;
    }

    Gene makeGene(uint16_t sourceType, uint16_t sourceNum, uint16_t sinkType, uint16_t sinkNum, uint16_t weight)
    {
        Gene gene;

        gene.sourceType = sourceType;
        gene.sourceNum = sourceNum;
        gene.sinkType = sinkType;
        gene.sinkNum = sinkNum;
        gene.weight = weight;
        return gene;
    }

} // namespace BS
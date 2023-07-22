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
        gene.weight =  randomUint(0, 0xffff) - 0x8000; //Gene::makeRandomWeight();


        return gene;
    }

} // namespace BS
#include "../include/gene.h"

namespace BS
{
    
    float Gene::weightAsFloat() const 
    { 
        return weight / 8192.0; 
    }

    bool Gene::operator== (const Gene& g1) const
    {
        return g1.sinkNum == sinkNum
            && g1.sourceNum == sourceNum
            && g1.sinkType == sinkType
            && g1.sourceType == sourceType
            && g1.weight == weight;
    }

    bool Gene::operator!= (const Gene& g1) const
    {
        return g1.sinkNum != sinkNum
            || g1.sourceNum != sourceNum
            || g1.sinkType != sinkType
            || g1.sourceType != sourceType
            || g1.weight != weight;
    }

} // namespace BS
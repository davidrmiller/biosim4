#ifndef CHALLENGEPAIRS_H_INCLUDED
#define CHALLENGEPAIRS_H_INCLUDED

#include <cassert>
#include "SurvivalCriteria.h"

namespace BS
{
    class ChallengePairs : public SurvivalCriteria
    {
        public:
            ChallengePairs();

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override;
    };    
}

#endif
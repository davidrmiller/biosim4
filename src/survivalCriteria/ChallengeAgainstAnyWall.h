#ifndef CHALLENGEAGAINSTANYWALL_H_INCLUDED
#define CHALLENGEAGAINSTANYWALL_H_INCLUDED

#include <cassert>
#include "SurvivalCriteria.h"

namespace BS
{
    class ChallengeAgainstAnyWall : public SurvivalCriteria
    {
        public:
            ChallengeAgainstAnyWall();

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override;
    };    
}

#endif
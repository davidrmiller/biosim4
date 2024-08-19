#ifndef CHALLENGENEARBARRIER_H_INCLUDED
#define CHALLENGENEARBARRIER_H_INCLUDED

#include <cassert>
#include "survivalCriteria.h"

namespace BS
{
    class ChallengeNearBarrier : public SurvivalCriteria
    {
        public:
            ChallengeNearBarrier();

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override;
    };    
}

#endif
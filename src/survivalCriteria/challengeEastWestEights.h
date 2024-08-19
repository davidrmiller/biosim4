#ifndef CHALLENGEEASTWESTEIGHTS_H_INCLUDED
#define CHALLENGEEASTWESTEIGHTS_H_INCLUDED

#include <cassert>
#include "survivalCriteria.h"

namespace BS
{
    class ChallengeEastWestEights : public SurvivalCriteria
    {
        public:
            ChallengeEastWestEights();
            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override;
            void initShapes(int liveDisplayScale) override;
    };    
}

#endif
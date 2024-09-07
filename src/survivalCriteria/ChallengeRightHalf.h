#ifndef CHALLENGERIGHTHALF_H_INCLUDED
#define CHALLENGERIGHTHALF_H_INCLUDED

#include "SurvivalCriteria.h"

namespace BS
{
    class ChallengeRightHalf : public SurvivalCriteria
    {
        public:
            ChallengeRightHalf();

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override;
            void initShapes(int liveDisplayScale) override;
    };    
}

#endif
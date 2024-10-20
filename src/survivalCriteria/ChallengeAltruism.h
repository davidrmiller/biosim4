#ifndef CHALLENGEALTRUISM_H_INCLUDED
#define CHALLENGEALTRUISM_H_INCLUDED

#include <cassert>
#include "SurvivalCriteria.h"

namespace BS
{
    class ChallengeAltruism : public SurvivalCriteria
    {
        public:
            ChallengeAltruism();

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override;
            void initShapes(int liveDisplayScale) override;

        private:
            float radiusPart;
            float getX();
            float getY();
    };    
}

#endif
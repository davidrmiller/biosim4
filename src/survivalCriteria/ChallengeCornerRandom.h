#ifndef CHALLENGECORNERRANDOM_H_INCLUDED
#define CHALLENGECORNERRANDOM_H_INCLUDED

#include <cassert>
#include <tuple>
#include "SurvivalCriteria.h"

namespace BS
{
    class ChallengeCornerRandom : public SurvivalCriteria
    {
        public:
            ChallengeCornerRandom();
            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override;
            void initShapes(int liveDisplayScale) override;
        private:
            float radiusPart = 4.0;
            bool cornerSurvival[4] = {true, true, true, true};
    };    
}

#endif
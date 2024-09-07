#ifndef CHALLENGECORNER_H_INCLUDED
#define CHALLENGECORNER_H_INCLUDED

#include <cassert>
#include "SurvivalCriteria.h"

namespace BS
{
    class ChallengeCorner : public SurvivalCriteria
    {
        public:
            ChallengeCorner();
            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override;
            void initShapes(int liveDisplayScale) override;
        private:
            float radiusPart = 8.0;
    };    
}

#endif
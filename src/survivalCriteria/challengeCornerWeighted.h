#ifndef CHALLENGECORNERWEIGHTED_H_INCLUDED
#define CHALLENGECORNERWEIGHTED_H_INCLUDED

#include <cassert>
#include "survivalCriteria.h"

namespace BS
{
    class ChallengeCornerWeighted : public SurvivalCriteria
    {
        public:
            ChallengeCornerWeighted();
            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override;
            void initShapes(int liveDisplayScale) override;
        private:
            float radiusPart = 4.0;
    };    
}

#endif
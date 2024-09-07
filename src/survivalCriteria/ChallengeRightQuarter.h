#ifndef CHALLENGERIGHTQUARTER_H_INCLUDED
#define CHALLENGERIGHTQUARTER_H_INCLUDED

#include "SurvivalCriteria.h"

namespace BS
{
    class ChallengeRightQuarter : public SurvivalCriteria
    {
        public:
            
            ChallengeRightQuarter();

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override;
            void initShapes(int liveDisplayScale) override;
    };    
}

#endif
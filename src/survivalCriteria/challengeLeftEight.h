#ifndef CHALLENGELEFTEIGHT_H_INCLUDED
#define CHALLENGELEFTEIGHT_H_INCLUDED

#include <cassert>
#include "survivalCriteria.h"

namespace BS
{
    class ChallengeLeftEight : public SurvivalCriteria
    {
        public:
            ChallengeLeftEight();
            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override;
            void initShapes(int liveDisplayScale) override;
    };    
}

#endif
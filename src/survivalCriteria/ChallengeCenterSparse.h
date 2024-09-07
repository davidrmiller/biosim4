#ifndef CHALLENGECENTERSPARSE_H_INCLUDED
#define CHALLENGECENTERSPARSE_H_INCLUDED

#include <cassert>
#include "SurvivalCriteria.h"

namespace BS
{
    class ChallengeCenterSparse : public SurvivalCriteria
    {
        public:
            ChallengeCenterSparse();
            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override;
            void initShapes(int liveDisplayScale) override;

        private:
            float radiusPart;
            float innerRadius;
            float getX();
            float getY();
    };    
}

#endif
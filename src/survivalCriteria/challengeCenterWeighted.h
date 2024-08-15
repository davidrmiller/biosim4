#ifndef CHALLENGECENTERWEIGHTED_H_INCLUDED
#define CHALLENGECENTERWEIGHTED_H_INCLUDED

#include "survivalCriteria.h"

namespace BS
{
    class ChallengeCenterWeighted : public SurvivalCriteria
    {
        public:
            ChallengeCenterWeighted();
            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override;
            void initShapes(int liveDisplayScale) override;

        private:
            float radiusPart;
            float getX();
            float getY();
    };
}

#endif
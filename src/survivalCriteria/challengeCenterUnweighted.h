#ifndef CHALLENGECENTERUNWEIGHTED_H_INCLUDED
#define CHALLENGECENTERUNWEIGHTED_H_INCLUDED

#include "survivalCriteria.h"

namespace BS
{
    class ChallengeCenterUnweighted : public SurvivalCriteria
    {
        public:
            ChallengeCenterUnweighted();

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override;
            void initShapes(int liveDisplayScale) override;

        private:
            float radiusPart;
            float getX();
            float getY();
    };    
}

#endif
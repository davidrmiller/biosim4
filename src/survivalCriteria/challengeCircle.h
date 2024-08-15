#ifndef CHALLENGECIRCLE_H_INCLUDED
#define CHALLENGECIRCLE_H_INCLUDED

#include "survivalCriteria.h"

namespace BS
{
    class ChallengeCircle : public SurvivalCriteria
    {
        public:
            ChallengeCircle();
            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override;
            void initShapes(int liveDisplayScale) override;

        private:
            float radiusPart;
            float getX();
            float getY();
    };
}

#endif
#ifndef CHALLENGERIGHTQUARTER_H_INCLUDED
#define CHALLENGERIGHTQUARTER_H_INCLUDED

#include "survivalCriteria.h"

namespace BS
{
    class ChallengeRightQuarter : public SurvivalCriteria
    {
        public:
            ChallengeRightQuarter()
            {
                this->value = CHALLENGE_RIGHT_QUARTER;
                this->text = "Right quarter";
                this->description = "Survivors are all those on the right quarter of the arena";
            }

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override
            {
                return indiv.loc.x > p.sizeX / 2 + p.sizeX / 4 ?
                    std::pair<bool, float> { true, 1.0 }
                    : std::pair<bool, float> { false, 0.0 };
            }
    };    
}

#endif
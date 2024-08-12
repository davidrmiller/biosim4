#ifndef CHALLENGERIGHTHALF_H_INCLUDED
#define CHALLENGERIGHTHALF_H_INCLUDED

#include "survivalCriteria.h"

namespace BS
{
    class ChallengeRightHalf : public SurvivalCriteria
    {
        public:
            ChallengeRightHalf()
            {
                this->value = CHALLENGE_RIGHT_HALF;
                this->text = "Right half";
                this->description = "Survivors are all those on the right side of the arena";
            }

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override
            {
                return indiv.loc.x > p.sizeX / 2 ?
                    std::pair<bool, float> { true, 1.0 }
                    : std::pair<bool, float> { false, 0.0 };
            }
    };    
}

#endif
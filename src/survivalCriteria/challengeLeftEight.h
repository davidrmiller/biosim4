#ifndef CHALLENGELEFTEIGHT_H_INCLUDED
#define CHALLENGELEFTEIGHT_H_INCLUDED

#include <cassert>
#include "survivalCriteria.h"

namespace BS
{
    class ChallengeLeftEight : public SurvivalCriteria
    {
        public:
            ChallengeLeftEight()
            {
                this->value = CHALLENGE_LEFT_EIGHTH;
                this->text = "Left eight";
                this->description = "Survivors are all those on the left eighth of the arena";
            }

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override
            {
                return indiv.loc.x < p.sizeX / 8 ?
                    std::pair<bool, float> { true, 1.0 }
                    : std::pair<bool, float> { false, 0.0 };
            }
    };    
}

#endif
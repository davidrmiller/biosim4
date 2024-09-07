#ifndef CHALLENGERADIOACTIVEWALLS_H_INCLUDED
#define CHALLENGERADIOACTIVEWALLS_H_INCLUDED

#include <cassert>
#include "SurvivalCriteria.h"

namespace BS
{
    class ChallengeRadioactiveWalls : public SurvivalCriteria
    {
        public:
            ChallengeRadioactiveWalls()
            {
                this->value = CHALLENGE_RADIOACTIVE_WALLS;
                this->text = "Radioactive walls";
                this->description = "During the first half of the generation, the west wall is radioactive, \
where X == 0. In the last half of the generation, the east wall is \
radioactive, where X = the area width - 1. There's an exponential \
falloff of the danger, falling off to zero at the arena half line";
            }

            // This challenge is handled in endOfSimStep(), where individuals may die
            // at the end of any sim step. There is nothing else to do here at the
            // end of a generation. All remaining alive become parents.
            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override
            {
                return { true, 1.0 };
            }
    };    
}

#endif
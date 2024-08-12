#ifndef CHALLENGEAGAINSTANYWALL_H_INCLUDED
#define CHALLENGEAGAINSTANYWALL_H_INCLUDED

#include <cassert>
#include "survivalCriteria.h"

namespace BS
{
    class ChallengeAgainstAnyWall : public SurvivalCriteria
    {
        public:
            ChallengeAgainstAnyWall()
            {
                this->value = CHALLENGE_AGAINST_ANY_WALL;
                this->text = "Against any wall";
                this->description = "Survivors are those touching any wall at the end of the generation";
            }

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override
            {
                bool onEdge = indiv.loc.x == 0 || indiv.loc.x == p.sizeX - 1
                    || indiv.loc.y == 0 || indiv.loc.y == p.sizeY - 1;

                if (onEdge) {
                    return { true, 1.0 };
                } else {
                    return { false, 0.0 };
                }
            }
    };    
}

#endif
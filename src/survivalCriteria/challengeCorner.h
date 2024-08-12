#ifndef CHALLENGECORNER_H_INCLUDED
#define CHALLENGECORNER_H_INCLUDED

#include <cassert>
#include "survivalCriteria.h"

namespace BS
{
    class ChallengeCorner : public SurvivalCriteria
    {
        public:
            ChallengeCorner()
            {
                this->value = CHALLENGE_CORNER;
                this->text = "Corner";
                this->description = "Survivors are those within the specified radius of any corner. \
Assumes square arena";
            }

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override
            {
                assert(p.sizeX == p.sizeY);
                float radius = p.sizeX / 8.0;

                float distance = (Coord(0, 0) - indiv.loc).length();
                if (distance <= radius) {
                    return { true, 1.0 };
                }
                distance = (Coord(0, p.sizeY - 1) - indiv.loc).length();
                if (distance <= radius) {
                    return { true, 1.0 };
                }
                distance = (Coord(p.sizeX - 1, 0) - indiv.loc).length();
                if (distance <= radius) {
                    return { true, 1.0 };
                }
                distance = (Coord(p.sizeX - 1, p.sizeY - 1) - indiv.loc).length();
                if (distance <= radius) {
                    return { true, 1.0 };
                }
                return { false, 0.0 };
            }
    };    
}

#endif
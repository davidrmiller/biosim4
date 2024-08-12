#ifndef CHALLENGECORNERWEIGHTED_H_INCLUDED
#define CHALLENGECORNERWEIGHTED_H_INCLUDED

#include <cassert>
#include "survivalCriteria.h"

namespace BS
{
    class ChallengeCornerWeighted : public SurvivalCriteria
    {
        public:
            ChallengeCornerWeighted()
            {
                this->value = CHALLENGE_CORNER_WEIGHTED;
                this->text = "Corner weighted";
                this->description = "Survivors are those within the specified radius of any corner. The score \
is linearly weighted by distance from the corner point";
            }

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override
            {
                assert(p.sizeX == p.sizeY);
                float radius = p.sizeX / 4.0;

                float distance = (Coord(0, 0) - indiv.loc).length();
                if (distance <= radius) {
                    return { true, (radius - distance) / radius };
                }
                distance = (Coord(0, p.sizeY - 1) - indiv.loc).length();
                if (distance <= radius) {
                    return { true, (radius - distance) / radius };
                }
                distance = (Coord(p.sizeX - 1, 0) - indiv.loc).length();
                if (distance <= radius) {
                    return { true, (radius - distance) / radius };
                }
                distance = (Coord(p.sizeX - 1, p.sizeY - 1) - indiv.loc).length();
                if (distance <= radius) {
                    return { true, (radius - distance) / radius };
                }
                return { false, 0.0 };
            }
    };    
}

#endif
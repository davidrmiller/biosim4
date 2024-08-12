#ifndef CHALLENGESTRING_H_INCLUDED
#define CHALLENGESTRING_H_INCLUDED

#include "survivalCriteria.h"

namespace BS
{
    class ChallengeString : public SurvivalCriteria
    {
        public:
            ChallengeString()
            {
                this->value = CHALLENGE_STRING;
                this->text = "String";
                this->description = "Survivors are those not touching the border and with exactly the number \
of neighbors defined by neighbors and radius, where neighbors includes self";
            }

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override
            {            
                unsigned minNeighbors = 22;
                unsigned maxNeighbors = 2;
                float radius = 1.5;

                if (grid.isBorder(indiv.loc)) {
                    return { false, 0.0 };
                }

                unsigned count = 0;
                auto f = [&](Coord loc2){
                    if (grid.isOccupiedAt(loc2)) ++count;
                };

                visitNeighborhood(indiv.loc, radius, f);
                if (count >= minNeighbors && count <= maxNeighbors) {
                    return { true, 1.0 };
                } else {
                    return { false, 0.0 };
                }
            }
    };    
}

#endif
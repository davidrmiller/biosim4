#ifndef CHALLENGECENTERSPARSE_H_INCLUDED
#define CHALLENGECENTERSPARSE_H_INCLUDED

#include <cassert>
#include "survivalCriteria.h"

namespace BS
{
    class ChallengeCenterSparse : public SurvivalCriteria
    {
        public:
            ChallengeCenterSparse()
            {
                this->value = CHALLENGE_CENTER_SPARSE;
                this->text = "Center sparse";
                this->description = "Survivors are those within the specified outer radius of the center and with \
the specified number of neighbors in the specified inner radius. \
The score is not weighted by distance from the center";
            }

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override
            {
                Coord safeCenter { (int16_t)(p.sizeX / 2.0), (int16_t)(p.sizeY / 2.0) };
                float outerRadius = p.sizeX / 4.0;
                float innerRadius = 1.5;
                unsigned minNeighbors = 5;  // includes self
                unsigned maxNeighbors = 8;

                Coord offset = safeCenter - indiv.loc;
                float distance = offset.length();
                if (distance <= outerRadius) {
                    unsigned count = 0;
                    auto f = [&](Coord loc2){
                        if (grid.isOccupiedAt(loc2)) ++count;
                    };

                    visitNeighborhood(indiv.loc, innerRadius, f);
                    if (count >= minNeighbors && count <= maxNeighbors) {
                        return { true, 1.0 };
                    }
                }
                return { false, 0.0 };
            }
    };    
}

#endif
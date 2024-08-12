#ifndef CHALLENGEMIGRATEDISTANCE_H_INCLUDED
#define CHALLENGEMIGRATEDISTANCE_H_INCLUDED

#include <cassert>
#include "survivalCriteria.h"

namespace BS
{
    class ChallengeMigrateDistance : public SurvivalCriteria
    {
        public:
            ChallengeMigrateDistance()
            {
                this->value = CHALLENGE_MIGRATE_DISTANCE;
                this->text = "Migrate distance";
                this->description = "Everybody survives and are candidate parents, but scored by how far \
they migrated from their birth location";
            }

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override
            {
                //unsigned requiredDistance = p.sizeX / 2.0;
                float distance = (indiv.loc - indiv.birthLoc).length();
                distance = distance / (float)(std::max(p.sizeX, p.sizeY));
                return { true, distance };
            }
    };    
}

#endif
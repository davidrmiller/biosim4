#include "ChallengeMigrateDistance.h"

namespace BS
{
    ChallengeMigrateDistance::ChallengeMigrateDistance()
    {
        this->value = CHALLENGE_MIGRATE_DISTANCE;
        this->text = "Migrate distance";
        this->description = "Everybody survives and are candidate parents, but scored by how far \
they migrated from their birth location";
    }

    std::pair<bool, float> ChallengeMigrateDistance::passed(const Indiv &indiv, const Params &p, Grid &grid)
    {
        //unsigned requiredDistance = p.sizeX / 2.0;
        float distance = (indiv.loc - indiv.birthLoc).length();
        distance = distance / (float)(std::max(p.sizeX, p.sizeY));
        return { true, distance };
    }  
}
#ifndef CHALLENGEMIGRATEDISTANCE_H_INCLUDED
#define CHALLENGEMIGRATEDISTANCE_H_INCLUDED

#include <cassert>
#include "SurvivalCriteria.h"

namespace BS
{
    class ChallengeMigrateDistance : public SurvivalCriteria
    {
        public:
            ChallengeMigrateDistance();

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override;
    };    
}

#endif
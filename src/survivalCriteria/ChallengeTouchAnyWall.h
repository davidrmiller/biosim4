#ifndef CHALLENGETOUCHANYWALL_H_INCLUDED
#define CHALLENGETOUCHANYWALL_H_INCLUDED

#include <cassert>
#include "SurvivalCriteria.h"

namespace BS
{
    class ChallengeTouchAnyWall : public SurvivalCriteria
    {
        public:
            ChallengeTouchAnyWall();

            // This challenge is partially handled in endOfStep(), where individuals
            // that are touching a wall are flagged in their Indiv record. They are
            // allowed to continue living. Here at the end of the generation, any that
            // never touch a wall will die. All that touched a wall at any time during
            // their life will become parents.
            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override;
            void endOfStep(unsigned simStep, const Params &p, Grid &grid, Peeps &peeps) override;
    };    
}

#endif
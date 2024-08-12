#ifndef CHALLENGETOUCHANYWALL_H_INCLUDED
#define CHALLENGETOUCHANYWALL_H_INCLUDED

#include <cassert>
#include "survivalCriteria.h"

namespace BS
{
    class ChallengeTouchAnyWall : public SurvivalCriteria
    {
        public:
            ChallengeTouchAnyWall()
            {
                this->value = CHALLENGE_TOUCH_ANY_WALL;
                this->text = "Touch any wall";
                this->description = "This challenge is partially handled in endOfSimStep(), where individuals \
that are touching a wall are flagged in their Indiv record. They are \
allowed to continue living. At the end of the generation, any that \
never touch a wall will die. All that touched a wall at any time during \
their life will become parents";
            }

            // This challenge is partially handled in endOfSimStep(), where individuals
            // that are touching a wall are flagged in their Indiv record. They are
            // allowed to continue living. Here at the end of the generation, any that
            // never touch a wall will die. All that touched a wall at any time during
            // their life will become parents.
            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override
            {
                if (indiv.challengeBits != 0) {
                    return { true, 1.0 };
                } else {
                    return { false, 0.0 };
                }
            }
    };    
}

#endif
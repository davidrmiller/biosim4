#include "ChallengeTouchAnyWall.h"

namespace BS
{
    ChallengeTouchAnyWall::ChallengeTouchAnyWall()
    {
        this->value = CHALLENGE_TOUCH_ANY_WALL;
        this->text = "Touch any wall";
        this->description = "This challenge is partially handled in endOfStep(), where individuals \
that are touching a wall are flagged in their Indiv record. They are \
allowed to continue living. At the end of the generation, any that \
never touch a wall will die. All that touched a wall at any time during \
their life will become parents";
    }

    // This challenge is partially handled in endOfStep(), where individuals
    // that are touching a wall are flagged in their Indiv record. They are
    // allowed to continue living. Here at the end of the generation, any that
    // never touch a wall will die. All that touched a wall at any time during
    // their life will become parents.
    std::pair<bool, float> ChallengeTouchAnyWall::passed(const Indiv &indiv, const Params &p, Grid &grid)
    {
        if (indiv.challengeBits != 0) {
            return { true, 1.0 };
        } else {
            return { false, 0.0 };
        }
    }

    // If the individual is touching any wall, we set its challengeFlag to true.
    // At the end of the generation, all those with the flag true will reproduce.
    void ChallengeTouchAnyWall::endOfStep(unsigned simStep, const Params &p, Grid &grid, Peeps &peeps) 
    {
        for (uint16_t index = 1; index <= p.population; ++index) { // index 0 is reserved
            Indiv &indiv = peeps[index];
            if (indiv.loc.x == 0 || indiv.loc.x == p.sizeX - 1
             || indiv.loc.y == 0 || indiv.loc.y == p.sizeY - 1) {
                indiv.challengeBits = true;
            }
        }
    }
}

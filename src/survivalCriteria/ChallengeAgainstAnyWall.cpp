#include "ChallengeAgainstAnyWall.h"

namespace BS
{
    ChallengeAgainstAnyWall::ChallengeAgainstAnyWall()
    {
        this->value = CHALLENGE_AGAINST_ANY_WALL;
        this->text = "Against any wall";
        this->description = "Survivors are those touching any wall at the end of the generation";
    }

    std::pair<bool, float> ChallengeAgainstAnyWall::passed(const Indiv &indiv, const Params &p, Grid &grid)
    {
        bool onEdge = indiv.loc.x == 0 || indiv.loc.x == p.sizeX - 1
            || indiv.loc.y == 0 || indiv.loc.y == p.sizeY - 1;

        if (onEdge) {
            return { true, 1.0 };
        } else {
            return { false, 0.0 };
        }
    }  
}
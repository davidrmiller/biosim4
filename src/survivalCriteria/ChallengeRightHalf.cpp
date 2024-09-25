#include "ChallengeRightHalf.h"

namespace BS
{
    ChallengeRightHalf::ChallengeRightHalf()
    {
        this->value = CHALLENGE_RIGHT_HALF;
        this->text = "Right half";
        this->description = "Survivors are all those on the right side of the arena";
    }

    std::pair<bool, float> ChallengeRightHalf::passed(const Indiv &indiv, const Params &p, Grid &grid)
    {
        return indiv.loc.x > p.sizeX / 2 ? std::pair<bool, float>{true, 1.0}
                                         : std::pair<bool, float>{false, 0.0};
    }
    
    void ChallengeRightHalf::initShapes(int liveDisplayScale)
    {
        this->createLine(
            sf::Vector2f(p.sizeX / 2 * liveDisplayScale, 0), 
            sf::Vector2f(p.sizeX / 2 * liveDisplayScale, p.sizeY * liveDisplayScale)
        );
    }
}
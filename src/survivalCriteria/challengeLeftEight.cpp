#include "challengeLeftEight.h"

namespace BS
{
    ChallengeLeftEight::ChallengeLeftEight()
    {
        this->value = CHALLENGE_LEFT_EIGHTH;
        this->text = "Left eight";
        this->description = "Survivors are all those on the left eighth of the arena";
    }

    std::pair<bool, float> ChallengeLeftEight::passed(const Indiv &indiv, const Params &p, Grid &grid)
    {
        return indiv.loc.x < p.sizeX / 8 ? std::pair<bool, float>{true, 1.0}
                                         : std::pair<bool, float>{false, 0.0};
    }    
    
    void ChallengeLeftEight::initShapes(int liveDisplayScale)
    {
        this->createLine(
            sf::Vector2f(p.sizeX / 8 * liveDisplayScale, 0), 
            sf::Vector2f(p.sizeX / 8 * liveDisplayScale, p.sizeY * liveDisplayScale)
        );
    }
}
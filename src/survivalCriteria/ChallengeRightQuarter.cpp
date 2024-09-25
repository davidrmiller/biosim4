#include "ChallengeRightQuarter.h"

namespace BS
{
    ChallengeRightQuarter::ChallengeRightQuarter()
    {
        this->value = CHALLENGE_RIGHT_QUARTER;
        this->text = "Right quarter";
        this->description = "Survivors are all those on the right quarter of the arena";
    }

    std::pair<bool, float> ChallengeRightQuarter::passed(const Indiv &indiv, const Params &p, Grid &grid)
    {
        return indiv.loc.x > p.sizeX / 2 + p.sizeX / 4 ? std::pair<bool, float>{true, 1.0}
                                                       : std::pair<bool, float>{false, 0.0};
    }
    
    void ChallengeRightQuarter::initShapes(int liveDisplayScale)
    {
        this->createLine(
            sf::Vector2f((p.sizeX / 2 + p.sizeX / 4) * liveDisplayScale, 0), 
            sf::Vector2f((p.sizeX / 2 + p.sizeX / 4) * liveDisplayScale, p.sizeY * liveDisplayScale)
        );
    }
}
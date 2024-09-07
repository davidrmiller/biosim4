#include <cassert>
#include "ChallengeEastWestEights.h"

namespace BS
{
    ChallengeEastWestEights::ChallengeEastWestEights()
    {
        this->value = CHALLENGE_EAST_WEST_EIGHTHS;
        this->text = "East west eights";
        this->description = "Survivors are all those on the left or right eighths of the arena";
    }

    std::pair<bool, float> ChallengeEastWestEights::passed(const Indiv &indiv, const Params &p, Grid &grid)
    {
        return indiv.loc.x < p.sizeX / 8 || indiv.loc.x >= (p.sizeX - p.sizeX / 8) 
            ? std::pair<bool, float>{true, 1.0}
            : std::pair<bool, float>{false, 0.0};
    }

    void ChallengeEastWestEights::initShapes(int liveDisplayScale)
    {
        this->createLine(
            sf::Vector2f(p.sizeX / 8 * liveDisplayScale, 0), 
            sf::Vector2f(p.sizeX / 8 * liveDisplayScale, p.sizeY * liveDisplayScale)
        );
        this->createLine(
            sf::Vector2f((p.sizeX - p.sizeX / 8) * liveDisplayScale, 0), 
            sf::Vector2f((p.sizeX - p.sizeX / 8) * liveDisplayScale, p.sizeY * liveDisplayScale)
        );
    }
}
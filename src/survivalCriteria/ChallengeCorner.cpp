#include "ChallengeCorner.h"

namespace BS
{
    ChallengeCorner::ChallengeCorner()
    {
        this->value = CHALLENGE_CORNER;
        this->text = "Corner";
        this->description = "Survivors are those within the specified radius of any corner. \
Assumes square arena";
    }

    std::pair<bool, float> ChallengeCorner::passed(const Indiv &indiv, const Params &p, Grid &grid)
    {
        assert(p.sizeX == p.sizeY);
        float radius = p.sizeX / this->radiusPart;

        float distance = (Coord(0, 0) - indiv.loc).length();
        if (distance <= radius)
        {
            return {true, 1.0};
        }
        distance = (Coord(0, p.sizeY - 1) - indiv.loc).length();
        if (distance <= radius)
        {
            return {true, 1.0};
        }
        distance = (Coord(p.sizeX - 1, 0) - indiv.loc).length();
        if (distance <= radius)
        {
            return {true, 1.0};
        }
        distance = (Coord(p.sizeX - 1, p.sizeY - 1) - indiv.loc).length();
        if (distance <= radius)
        {
            return {true, 1.0};
        }
        return {false, 0.0};
    }

    void ChallengeCorner::initShapes(int liveDisplayScale)
    {
        float radius = p.sizeX / this->radiusPart * liveDisplayScale;
        this->createCircle(radius, 0 - radius, 0 - radius);
        this->createCircle(radius, 0 - radius, (p.sizeY * liveDisplayScale) - radius);
        this->createCircle(radius, (p.sizeX * liveDisplayScale) - radius, 0 - radius);
        this->createCircle(radius, (p.sizeX * liveDisplayScale) - radius, (p.sizeY * liveDisplayScale) - radius);
        this->createBorder(radius + 10, liveDisplayScale);
    }
}
#include "ChallengeCornerWeighted.h"

namespace BS
{
    ChallengeCornerWeighted::ChallengeCornerWeighted()
    {
        this->value = CHALLENGE_CORNER_WEIGHTED;
        this->text = "Corner weighted";
        this->description = "Survivors are those within the specified radius of any corner. The score \
is linearly weighted by distance from the corner point";
    }

    std::pair<bool, float> ChallengeCornerWeighted::passed(const Indiv &indiv, const Params &p, Grid &grid)
    {
        assert(p.sizeX == p.sizeY);
        float radius = p.sizeX / this->radiusPart;
        float distance = 0;

        distance = (Coord(0, 0) - indiv.loc).length();
        if (distance <= radius)
        {
            return {true, (radius - distance) / radius};
        }
        distance = (Coord(0, p.sizeY - 1) - indiv.loc).length();
        if (distance <= radius)
        {
            return {true, (radius - distance) / radius};
        }
        distance = (Coord(p.sizeX - 1, 0) - indiv.loc).length();
        if (distance <= radius)
        {
            return {true, (radius - distance) / radius};
        }
        distance = (Coord(p.sizeX - 1, p.sizeY - 1) - indiv.loc).length();
        if (distance <= radius)
        {
            return {true, (radius - distance) / radius};
        }
        return {false, 0.0};
    }

    void ChallengeCornerWeighted::initShapes(int liveDisplayScale)
    {
        float radius = p.sizeX / this->radiusPart * liveDisplayScale;
        this->createCircle(radius, 0 - radius, 0 - radius);
        this->createCircle(radius, 0 - radius, (p.sizeY * liveDisplayScale) - radius);
        this->createCircle(radius, (p.sizeX * liveDisplayScale) - radius, 0 - radius);
        this->createCircle(radius, (p.sizeX * liveDisplayScale) - radius, (p.sizeY * liveDisplayScale) - radius);
        this->createBorder(radius + 10, liveDisplayScale);
    }
}
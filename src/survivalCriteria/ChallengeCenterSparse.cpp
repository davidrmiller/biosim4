
#include "ChallengeCenterSparse.h"

namespace BS
{
    ChallengeCenterSparse::ChallengeCenterSparse()
    {
        this->value = CHALLENGE_CENTER_SPARSE;
        this->text = "Center sparse";
        this->description = "Survivors are those within the specified outer radius of the center and with \
the specified number of neighbors in the specified inner radius. \
The score is not weighted by distance from the center";

        this->radiusPart = 4.0;
        this->innerRadius = 1.5;
    }

    std::pair<bool, float> ChallengeCenterSparse::passed(const Indiv &indiv, const Params &p, Grid &grid)
    {
        Coord safeCenter{(int16_t)this->getX(), (int16_t)this->getY()};
        float outerRadius = p.sizeX / this->radiusPart;
        float innerRadius = this->innerRadius;
        unsigned minNeighbors = 5; // includes self
        unsigned maxNeighbors = 8;

        Coord offset = safeCenter - indiv.loc;
        float distance = offset.length();
        if (distance <= outerRadius)
        {
            unsigned count = 0;
            auto f = [&](Coord loc2)
            {
                if (grid.isOccupiedAt(loc2))
                    ++count;
            };

            visitNeighborhood(indiv.loc, innerRadius, f);
            if (count >= minNeighbors && count <= maxNeighbors)
            {
                return {true, 1.0};
            }
        }
        return {false, 0.0};
    }

    void ChallengeCenterSparse::initShapes(int liveDisplayScale)
    {
        float radius = p.sizeX / this->radiusPart * liveDisplayScale;
        float innerRadius = this->innerRadius * liveDisplayScale;
        this->createCircle(radius, this->getX() * liveDisplayScale - radius, (p.sizeY - this->getY()) * liveDisplayScale - radius);
        this->createCircle(innerRadius, this->getX() * liveDisplayScale - innerRadius, (p.sizeY - this->getY()) * liveDisplayScale - innerRadius);
    }

    float ChallengeCenterSparse::getX()
    {
        return p.sizeX / 2.0;
    }

    float ChallengeCenterSparse::getY()
    {
        return p.sizeY / 2.0;
    }
}
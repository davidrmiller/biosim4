#include "challengeCenterWeighted.h"

namespace BS
{
    ChallengeCenterWeighted::ChallengeCenterWeighted()
    {
        this->value = CHALLENGE_CENTER_WEIGHTED;
        this->text = "Center weighted";
        this->description = "Survivors are those within the specified radius of the center. The score \
is linearly weighted by distance from the center";

        this->radiusPart = 3.0;
    }

    std::pair<bool, float> ChallengeCenterWeighted::passed(const Indiv &indiv, const Params &p, Grid &grid)
    {
        Coord safeCenter{(int16_t)this->getX(), (int16_t)this->getY()};
        float radius = p.sizeX / this->radiusPart;

        Coord offset = safeCenter - indiv.loc;
        float distance = offset.length();
        return distance <= radius ? std::pair<bool, float>{true, (radius - distance) / radius}
                                  : std::pair<bool, float>{false, 0.0};
    }
        
    void ChallengeCenterWeighted::initShapes(int liveDisplayScale)
    {
        float radius = p.sizeX / this->radiusPart * liveDisplayScale;
        this->createCircle(radius, this->getX() * liveDisplayScale - radius, (p.sizeY - this->getY()) * liveDisplayScale - radius);
    }

    float ChallengeCenterWeighted::getX()
    {
        return p.sizeX / 2.0;
    }

    float ChallengeCenterWeighted::getY()
    {
        return p.sizeY / 2.0;
    }
}
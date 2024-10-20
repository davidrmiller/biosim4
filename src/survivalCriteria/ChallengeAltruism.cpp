#include "ChallengeAltruism.h"

namespace BS
{
    ChallengeAltruism::ChallengeAltruism()
    {
        this->value = CHALLENGE_ALTRUISM;
        this->text = "Alruism";
        this->description = "Survivors are those inside the circular area defined by \
safeCenter and radius";

        this->radiusPart = 4.0; // in 128^2 world, holds 804 agents
    }

    std::pair<bool, float> ChallengeAltruism::passed(const Indiv &indiv, const Params &p, Grid &grid)
    {      
        Coord safeCenter { (int16_t)(this->getX()), (int16_t)(this->getY()) };
        float radius = p.sizeX / this->radiusPart;

        Coord offset = safeCenter - indiv.loc;
        float distance = offset.length();
        return distance <= radius ?
            std::pair<bool, float> { true, (radius - distance) / radius }
            : std::pair<bool, float> { false, 0.0 };
    }  

    void ChallengeAltruism::initShapes(int liveDisplayScale)
    {
        float radius = p.sizeX / this->radiusPart * liveDisplayScale;
        this->createCircle(radius, this->getX() * liveDisplayScale - radius, (p.sizeY - this->getY()) * liveDisplayScale - radius);
    }

    float ChallengeAltruism::getX()
    {
        return p.sizeX / 4.0;
    }

    float ChallengeAltruism::getY()
    {
        return  p.sizeY / 4.0;
    }
}
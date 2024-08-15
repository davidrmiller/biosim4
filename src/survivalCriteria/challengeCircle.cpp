#include "challengeCircle.h"

namespace BS
{
    ChallengeCircle::ChallengeCircle()
    {
        this->value = CHALLENGE_CIRCLE;
        this->text = "Circle";
        this->description = "Survivors are those inside the circular area defined \
by safeCenter and radius";

        this->radiusPart = 4.0;
    }

    std::pair<bool, float> ChallengeCircle::passed(const Indiv &indiv, const Params &p, Grid &grid)
    {
        Coord safeCenter { (int16_t)this->getX(), (int16_t)this->getY() };
        float radius = p.sizeX / this->radiusPart;

        Coord offset = safeCenter - indiv.loc;
        float distance = offset.length();
        return distance <= radius ?
            std::pair<bool, float> { true, (radius - distance) / radius }
            : std::pair<bool, float> { false, 0.0 };
    }

    void ChallengeCircle::initShapes(int liveDisplayScale)
    {
        float radius = p.sizeX / this->radiusPart * liveDisplayScale;
        this->createCircle(radius, this->getX() * liveDisplayScale - radius, (p.sizeY - this->getY()) * liveDisplayScale - radius);
    }

    float ChallengeCircle::getX()
    {
        return p.sizeX / 4.0;
    }

    float ChallengeCircle::getY()
    {
        return p.sizeY / 4.0;
    }
}
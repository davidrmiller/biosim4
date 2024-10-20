#include "ChallengeAltruismSacrifice.h"

namespace BS
{
    ChallengeAltruismSacrifice::ChallengeAltruismSacrifice()
    {
        this->value = CHALLENGE_ALTRUISM_SACRIFICE;
        this->text = "Alruism sacrifice";
        this->description = "Survivors are all those within the specified radius of the NE corner";

        this->radiusPart = 4.0; // in 128^2 world, holds 804 agents
    }

    std::pair<bool, float> ChallengeAltruismSacrifice::passed(const Indiv &indiv, const Params &p, Grid &grid)
    {   
        float radius = p.sizeX / this->radiusPart; 

        float distance = (Coord(this->getX(), this->getY()) - indiv.loc).length();
        if (distance <= radius) {
            return { true, (radius - distance) / radius };
        } else {
            return { false, 0.0 };
        }
    }
    
    void ChallengeAltruismSacrifice::initShapes(int liveDisplayScale)
    {
        float radius = p.sizeX / this->radiusPart * liveDisplayScale;
        this->createCircle(radius, this->getX() * liveDisplayScale - radius, (p.sizeY - this->getY()) * liveDisplayScale - radius);
    }

    float ChallengeAltruismSacrifice::getX()
    {
        return p.sizeX - p.sizeX / 4;
    }

    float ChallengeAltruismSacrifice::getY()
    {
        return  p.sizeY - p.sizeY / 4;
    }
}

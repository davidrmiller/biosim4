#ifndef CHALLENGEALTRUISMSACRIFICE_H_INCLUDED
#define CHALLENGEALTRUISMSACRIFICE_H_INCLUDED

#include <cassert>
#include "SurvivalCriteria.h"

namespace BS
{
    class ChallengeAltruismSacrifice : public SurvivalCriteria
    {
        public:
            ChallengeAltruismSacrifice()
            {
                this->value = CHALLENGE_ALTRUISM_SACRIFICE;
                this->text = "Alruism sacrifice";
                this->description = "Survivors are all those within the specified radius of the NE corner";
            }

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override
            {      
                //float radius = p.sizeX / 3.0; // in 128^2 world, holds 1429 agents
                float radius = p.sizeX / 4.0; // in 128^2 world, holds 804 agents
                //float radius = p.sizeX / 5.0; // in 128^2 world, holds 514 agents

                float distance = (Coord(p.sizeX - p.sizeX / 4, p.sizeY - p.sizeY / 4) - indiv.loc).length();
                if (distance <= radius) {
                    return { true, (radius - distance) / radius };
                } else {
                    return { false, 0.0 };
                }
            }
    };    
}

#endif
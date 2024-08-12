#ifndef CHALLENGEALTRUISM_H_INCLUDED
#define CHALLENGEALTRUISM_H_INCLUDED

#include <cassert>
#include "survivalCriteria.h"

namespace BS
{
    class ChallengeAltruism : public SurvivalCriteria
    {
        public:
            ChallengeAltruism()
            {
                this->value = CHALLENGE_ALTRUISM;
                this->text = "Alruism";
                this->description = "Survivors are those inside the circular area defined by \
safeCenter and radius";
            }

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override
            {      
                Coord safeCenter { (int16_t)(p.sizeX / 4.0), (int16_t)(p.sizeY / 4.0) };
                float radius = p.sizeX / 4.0; // in a 128^2 world, holds 3216

                Coord offset = safeCenter - indiv.loc;
                float distance = offset.length();
                return distance <= radius ?
                    std::pair<bool, float> { true, (radius - distance) / radius }
                    : std::pair<bool, float> { false, 0.0 };
            }
    };    
}

#endif
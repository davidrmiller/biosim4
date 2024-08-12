#ifndef CHALLENGECIRCLE_H_INCLUDED
#define CHALLENGECIRCLE_H_INCLUDED

#include "survivalCriteria.h"

namespace BS
{
    class ChallengeCircle : public SurvivalCriteria
    {
        public:
            ChallengeCircle()
            {
                this->value = CHALLENGE_CIRCLE;
                this->text = "Circle";
                this->description = "Survivors are those inside the circular area defined \
by safeCenter and radius";
            }

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override
            {
                Coord safeCenter { (int16_t)(p.sizeX / 4.0), (int16_t)(p.sizeY / 4.0) };
                float radius = p.sizeX / 4.0;

                Coord offset = safeCenter - indiv.loc;
                float distance = offset.length();
                return distance <= radius ?
                    std::pair<bool, float> { true, (radius - distance) / radius }
                    : std::pair<bool, float> { false, 0.0 };
            }
    };    
}

#endif
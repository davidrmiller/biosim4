#ifndef CHALLENGECENTERWEIGHTED_H_INCLUDED
#define CHALLENGECENTERWEIGHTED_H_INCLUDED

#include "survivalCriteria.h"

namespace BS
{
    class ChallengeCenterWeighted : public SurvivalCriteria
    {
        public:
            ChallengeCenterWeighted()
            {
                this->value = CHALLENGE_CENTER_WEIGHTED;
                this->text = "Center weighted";
                this->description = "Survivors are those within the specified radius of the center. The score \
is linearly weighted by distance from the center";
            }

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override
            {
                Coord safeCenter { (int16_t)(p.sizeX / 2.0), (int16_t)(p.sizeY / 2.0) };
                float radius = p.sizeX / 3.0;

                Coord offset = safeCenter - indiv.loc;
                float distance = offset.length();
                return distance <= radius ?
                    std::pair<bool, float> { true, (radius - distance) / radius }
                    : std::pair<bool, float> { false, 0.0 };
            }
    };    
}

#endif
#ifndef CHALLENGECENTERUNWEIGHTED_H_INCLUDED
#define CHALLENGECENTERUNWEIGHTED_H_INCLUDED

#include "survivalCriteria.h"

namespace BS
{
    class ChallengeCenterUnweighted : public SurvivalCriteria
    {
        public:
            ChallengeCenterUnweighted()
            {
                this->value = CHALLENGE_CENTER_WEIGHTED;
                this->text = "Center unweighted";
                this->description = "Survivors are those within the specified radius of the center";
            }

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override
            {
                Coord safeCenter { (int16_t)(p.sizeX / 2.0), (int16_t)(p.sizeY / 2.0) };
                float radius = p.sizeX / 3.0;

                Coord offset = safeCenter - indiv.loc;
                float distance = offset.length();
                return distance <= radius ?
                    std::pair<bool, float> { true, 1.0 }
                    : std::pair<bool, float> { false, 0.0 };
            }
    };    
}

#endif
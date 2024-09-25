#include "ChallengeRadioactiveWalls.h"

namespace BS
{    
    ChallengeRadioactiveWalls::ChallengeRadioactiveWalls()
    {
        this->value = CHALLENGE_RADIOACTIVE_WALLS;
        this->text = "Radioactive walls";
        this->description = "During the first half of the generation, the west wall is radioactive, \
where X == 0. In the last half of the generation, the east wall is \
radioactive, where X = the area width - 1. There's an exponential \
falloff of the danger, falling off to zero at the arena half line";
    }

    // This challenge is handled in endOfStep(), where individuals may die
    // at the end of any sim step. There is nothing else to do here at the
    // end of a generation. All remaining alive become parents.
    std::pair<bool, float> ChallengeRadioactiveWalls::passed(const Indiv &indiv, const Params &p, Grid &grid)
    {
        return { true, 1.0 };
    }    
    
    void ChallengeRadioactiveWalls::endOfStep(unsigned simStep, const Params &p, Grid &grid, Peeps &peeps) 
    {
        // During the first half of the generation, the west wall is radioactive,
        // where X == 0. In the last half of the generation, the east wall is
        // radioactive, where X = the area width - 1. There's an exponential
        // falloff of the danger, falling off to zero at the arena half line.
        int16_t radioactiveX = (simStep < p.stepsPerGeneration / 2) ? 0 : p.sizeX - 1;

        for (uint16_t index = 1; index <= p.population; ++index) { // index 0 is reserved
            Indiv &indiv = peeps[index];
            if (indiv.alive) {
                int16_t distanceFromRadioactiveWall = std::abs(indiv.loc.x - radioactiveX);
                if (distanceFromRadioactiveWall < p.sizeX / 2) {
                    float chanceOfDeath = 1.0 / distanceFromRadioactiveWall;
                    if (randomUint() / (float)RANDOM_UINT_MAX < chanceOfDeath) {
                        peeps.queueForDeath(indiv);
                    }
                }
            }
        }
    }
}
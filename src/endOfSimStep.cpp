// endOfSimStep.cpp

#include <iostream>
#include <cmath>
#include "simulator.h"
#include "imageWriter.h"

namespace BS {

/*
At the end of each sim step, this function is called in single-thread
mode to take care of several things:

1. We may kill off some agents if a "radioactive" scenario is in progress.
2. We may flag some agents as meeting some challenge criteria, if such
   a scenario is in progress.
3. We then drain the deferred death queue.
4. We then drain the deferred movement queue.
5. We fade the signal layer(s) (pheromones).
6. We save the resulting world condition as a single image frame (if
   p.saveVideo is true).
*/

void endOfSimStep(unsigned simStep, unsigned generation)
{
    if (p.challenge == CHALLENGE_RADIOACTIVE_WALLS) {
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

    // If the individual is touching any wall, we set its challengeFlag to true.
    // At the end of the generation, all those with the flag true will reproduce.
    if (p.challenge == CHALLENGE_TOUCH_ANY_WALL) {
        for (uint16_t index = 1; index <= p.population; ++index) { // index 0 is reserved
            Indiv &indiv = peeps[index];
            if (indiv.loc.x == 0 || indiv.loc.x == p.sizeX - 1
             || indiv.loc.y == 0 || indiv.loc.y == p.sizeY - 1) {
                indiv.challengeBits = true;
            }
        }
    }

    // If this challenge is enabled, the individual gets a bit set in their challengeBits
    // member if they are within a specified radius of a barrier center. They have to
    // visit the barriers in sequential order.
    if (p.challenge == CHALLENGE_LOCATION_SEQUENCE) {
        float radius = 9.0;
        for (uint16_t index = 1; index <= p.population; ++index) { // index 0 is reserved
            Indiv &indiv = peeps[index];
            for (unsigned n = 0; n < grid.getBarrierCenters().size(); ++n) {
                unsigned bit = 1 << n;
                if ((indiv.challengeBits & bit) == 0) {
                    if ((indiv.loc - grid.getBarrierCenters()[n]).length() <= radius) {
                        indiv.challengeBits |= bit;
                    }
                    break;
                }
            }
        }
    }

    peeps.drainDeathQueue();
    peeps.drainMoveQueue();
    signals.fade(0); // takes layerNum  todo!!!

    // saveVideoFrameSync() is the synchronous version of saveVideFrame()
    if (p.saveVideo &&
                ((generation % p.videoStride) == 0
                 || generation <= p.videoSaveFirstFrames
                 || (generation >= p.parameterChangeGenerationNumber
                     && generation <= p.parameterChangeGenerationNumber + p.videoSaveFirstFrames))) {
        if (!imageWriter.saveVideoFrameSync(simStep, generation)) {
            std::cout << "imageWriter busy" << std::endl;
        }
    }
}

} // end namespace BS

#include "challengeNearBarrier.h"

namespace BS
{
            ChallengeNearBarrier::ChallengeNearBarrier()
            {
                this->value = CHALLENGE_NEAR_BARRIER;
                this->text = "Near barrier";
                this->description = "Survivors are those within radius of any barrier center. Weighted by distance";
            }

            std::pair<bool, float> ChallengeNearBarrier::passed(const Indiv &indiv, const Params &p, Grid &grid)
            {            
                float radius;
                radius = 10.0;
                //radius = p.sizeX / 2;
                //radius = p.sizeX / 4;

                const std::vector<Coord> barrierCenters = grid.getBarrierCenters();
                float minDistance = 1e8;
                for (auto& center : barrierCenters) {
                    float distance = (indiv.loc - center).length();
                    if (distance < minDistance) {
                        minDistance = distance;
                    }
                }
                if (minDistance <= radius) {
                    return { true, 1.0 - (minDistance / radius) };
                } else {
                    return { false, 0.0 };
                }
            }
    
}
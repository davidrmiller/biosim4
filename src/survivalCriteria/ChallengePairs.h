#ifndef CHALLENGEPAIRS_H_INCLUDED
#define CHALLENGEPAIRS_H_INCLUDED

#include <cassert>
#include "SurvivalCriteria.h"

namespace BS
{
    class ChallengePairs : public SurvivalCriteria
    {
        public:
            ChallengePairs()
            {
                this->value = CHALLENGE_PAIRS;
                this->text = "Pairs";
                this->description = "Survivors are those not touching a border and with exactly one neighbor which has no other neighbor";
            }

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override
            {            
                bool onEdge = indiv.loc.x == 0 || indiv.loc.x == p.sizeX - 1
                        || indiv.loc.y == 0 || indiv.loc.y == p.sizeY - 1;

                if (onEdge) {
                    return { false, 0.0 };
                }

                unsigned count = 0;
                for (int16_t x = indiv.loc.x - 1; x <= indiv.loc.x + 1; ++x) {
                    for (int16_t y = indiv.loc.y - 1; y <= indiv.loc.y + 1; ++y) {
                        Coord tloc = { x, y };
                        if (tloc != indiv.loc && grid.isInBounds(tloc) && grid.isOccupiedAt(tloc)) {
                            ++count;
                            if (count == 1) {
                                for (int16_t x1 = tloc.x - 1; x1 <= tloc.x + 1; ++x1) {
                                    for (int16_t y1 = tloc.y - 1; y1 <= tloc.y + 1; ++y1) {
                                        Coord tloc1 = { x1, y1 };
                                        if (tloc1 != tloc && tloc1 != indiv.loc && grid.isInBounds(tloc1) && grid.isOccupiedAt(tloc1)) {
                                            return { false, 0.0 };
                                        }
                                    }
                                }
                            } else {
                                return { false, 0.0 };
                            }
                        }
                    }
                }
                if (count == 1) {
                    return { true, 1.0 };
                } else {
                    return { false, 0.0 };
                }
            }
    };    
}

#endif
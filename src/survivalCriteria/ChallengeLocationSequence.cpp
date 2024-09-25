#include "ChallengeLocationSequence.h"

namespace BS
{
    ChallengeLocationSequence::ChallengeLocationSequence()
    {
        this->value = CHALLENGE_LOCATION_SEQUENCE;
        this->text = "Location sequence";
        this->description = "Survivors are those that contacted one or more specified locations in a sequence, \
ranked by the number of locations contacted. There will be a bit set in their \
challengeBits member for each location contacted";
    }

    std::pair<bool, float> ChallengeLocationSequence::passed(const Indiv &indiv, const Params &p, Grid &grid)
    {            
        unsigned count = 0;
        unsigned bits = indiv.challengeBits;
        unsigned maxNumberOfBits = sizeof(bits) * 8;

        for (unsigned n = 0; n < maxNumberOfBits; ++n) {
            if ((bits & (1 << n)) != 0) {
                ++count;
            }
        }
        if (count > 0) {
            return { true, count / (float)maxNumberOfBits };
        } else {
            return { false, 0.0 };
        }
    }

    // If this challenge is enabled, the individual gets a bit set in their challengeBits
    // member if they are within a specified radius of a barrier center. They have to
    // visit the barriers in sequential order.
    void ChallengeLocationSequence::endOfStep(unsigned simStep, const Params &p, Grid &grid, Peeps &peeps) 
    {
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
}
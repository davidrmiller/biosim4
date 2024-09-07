#ifndef CHALLENGELOCATIONSEQUENCE_H_INCLUDED
#define CHALLENGELOCATIONSEQUENCE_H_INCLUDED

#include <cassert>
#include "SurvivalCriteria.h"

namespace BS
{
    class ChallengeLocationSequence : public SurvivalCriteria
    {
        public:
            ChallengeLocationSequence()
            {
                this->value = CHALLENGE_LOCATION_SEQUENCE;
                this->text = "Location sequence";
                this->description = "Survivors are those that contacted one or more specified locations in a sequence, \
ranked by the number of locations contacted. There will be a bit set in their \
challengeBits member for each location contacted";
            }

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override
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
    };    
}

#endif
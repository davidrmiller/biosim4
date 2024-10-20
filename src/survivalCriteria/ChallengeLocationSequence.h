#ifndef CHALLENGELOCATIONSEQUENCE_H_INCLUDED
#define CHALLENGELOCATIONSEQUENCE_H_INCLUDED

#include <cassert>
#include "SurvivalCriteria.h"

namespace BS
{
    class ChallengeLocationSequence : public SurvivalCriteria
    {
        public:
            ChallengeLocationSequence();

            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid);
            void endOfStep(unsigned simStep, const Params &p, Grid &grid, Peeps &peeps) override;
    };    
}

#endif
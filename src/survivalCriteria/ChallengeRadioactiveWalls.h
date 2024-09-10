#ifndef CHALLENGERADIOACTIVEWALLS_H_INCLUDED
#define CHALLENGERADIOACTIVEWALLS_H_INCLUDED

#include <cassert>
#include "SurvivalCriteria.h"

namespace BS
{
    class ChallengeRadioactiveWalls : public SurvivalCriteria
    {
        public:
            ChallengeRadioactiveWalls();

            // This challenge is handled in endOfStep(), where individuals may die
            // at the end of any sim step. There is nothing else to do here at the
            // end of a generation. All remaining alive become parents.
            std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) override;

            void endOfStep(unsigned simStep, const Params &p, Grid &grid, Peeps &peeps) override;
    };    
}

#endif
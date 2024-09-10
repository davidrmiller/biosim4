#ifndef SURVIVALCRITERIAMANAGER_H_INCLUDED
#define SURVIVALCRITERIAMANAGER_H_INCLUDED

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "../ai/signals.h"

#include "SurvivalCriteria.h"
#include "ChallengeCircle.h"
#include "ChallengeRightHalf.h"
#include "ChallengeRightQuarter.h"
#include "ChallengeString.h"
#include "ChallengeCenterWeighted.h"
#include "ChallengeCenterUnweighted.h"
#include "ChallengeCorner.h"
#include "ChallengeCornerWeighted.h"
#include "ChallengeMigrateDistance.h"
#include "ChallengeCenterSparse.h"
#include "ChallengeLeftEight.h"
#include "ChallengeRadioactiveWalls.h"
#include "ChallengeAgainstAnyWall.h"
#include "ChallengeTouchAnyWall.h"
#include "ChallengeEastWestEights.h"
#include "ChallengeNearBarrier.h"
#include "ChallengePairs.h"
#include "ChallengeLocationSequence.h"
#include "ChallengeAltruism.h"
#include "ChallengeAltruismSacrifice.h"
#include "ChallengeCornerRandom.h"

namespace BS
{
    class SurvivalCriteriaManager
    {
        public:
            std::vector<SurvivalCriteria*> survivalCriteriasVector;
            SurvivalCriteria* currentChallenge = nullptr;
            SurvivalCriteriaManager();

            void startNewGeneration(unsigned challengeId);
            void initShapes(int liveDisplayScale);

            // Returns true and a score 0.0..1.0 if passed, false if failed
            std::pair<bool, float> passedSurvivalCriterion(const Indiv &indiv, const Params &p, Grid &grid);
            std::pair<bool, float> passedSurvivalCriterion(const Indiv &indiv, const Params &p, Grid &grid, unsigned challenge);

            std::vector<sf::Drawable*> getShapes();
            void endOfStep(unsigned simStep, const Params &p, Grid &grid, Peeps &peeps, Signals signals);

        private:
            SurvivalCriteria* getById(unsigned id);
    };    
}

#endif
#ifndef SURVIVALCRITERIAMANAGER_H_INCLUDED
#define SURVIVALCRITERIAMANAGER_H_INCLUDED

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "survivalCriteria.h"
#include "challengeCircle.h"
#include "challengeRightHalf.h"
#include "challengeRightQuarter.h"
#include "challengeString.h"
#include "challengeCenterWeighted.h"
#include "challengeCenterUnweighted.h"
#include "challengeCorner.h"
#include "challengeCornerWeighted.h"
#include "challengeMigrateDistance.h"
#include "challengeCenterSparse.h"
#include "challengeLeftEight.h"
#include "challengeRadioactiveWalls.h"
#include "challengeAgainstAnyWall.h"
#include "challengeTouchAnyWall.h"
#include "challengeEastWestEights.h"
#include "challengeNearBarrier.h"
#include "challengePairs.h"
#include "challengeLocationSequence.h"
#include "challengeAltruism.h"
#include "challengeAltruismSacrifice.h"

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

            // Returns true and a score 0.0..1.0 if passed, false if failed
            std::pair<bool, float> passedSurvivalCriterion(const Indiv &indiv, const Params &p, Grid &grid, unsigned challenge);

            std::vector<sf::Drawable*> getShapes();

        private:
            SurvivalCriteria* getById(unsigned id);
    };    
}

#endif
#ifndef SURVIVALCRITERIAMANAGER_H_INCLUDED
#define SURVIVALCRITERIAMANAGER_H_INCLUDED

#include <string>
#include <vector>
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
            SurvivalCriteria* currentChallenge;
            SurvivalCriteriaManager() 
            {
                this->survivalCriteriasVector = {
                    new ChallengeCircle(),
                    new ChallengeRightHalf(),
                    new ChallengeRightQuarter(),
                    new ChallengeString(),
                    new ChallengeCenterWeighted(),
                    new ChallengeCenterUnweighted(),
                    new ChallengeCorner(),
                    new ChallengeCornerWeighted(),
                    new ChallengeMigrateDistance(),
                    new ChallengeCenterSparse(),
                    new ChallengeLeftEight(),
                    new ChallengeRadioactiveWalls(),
                    new ChallengeAgainstAnyWall(),
                    new ChallengeTouchAnyWall(),
                    new ChallengeEastWestEights(),
                    new ChallengeNearBarrier(),
                    new ChallengePairs(),
                    new ChallengeLocationSequence(),
                    new ChallengeAltruism(),
                    new ChallengeAltruismSacrifice()
                };
            }

            void setChallenge(unsigned id)
            {
                this->currentChallenge = this->getById(id);
            }

            // Returns true and a score 0.0..1.0 if passed, false if failed
            std::pair<bool, float> passedSurvivalCriterion(const Indiv &indiv, const Params &p, Grid &grid)
            {
                if (!indiv.alive) {
                    return { false, 0.0 };
                }
                return this->currentChallenge->passed(indiv, p, grid);
            }

            // Returns true and a score 0.0..1.0 if passed, false if failed
            std::pair<bool, float> passedSurvivalCriterion(const Indiv &indiv, const Params &p, Grid &grid, unsigned challenge)
            {
                if (!indiv.alive) {
                    return { false, 0.0 };
                }
                return this->getById(challenge)->passed(indiv, p, grid);
            }

        private:
            SurvivalCriteria* getById(unsigned id)
            {
                for (auto survivalCriteria : survivalCriteriasVector) {
                    if (survivalCriteria->value == id) {
                        return survivalCriteria;
                    }
                }
                return this->survivalCriteriasVector[0];
            }
    };    
}

#endif
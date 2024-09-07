#include "SurvivalCriteriaManager.h"

namespace BS
{
    SurvivalCriteriaManager::SurvivalCriteriaManager()
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
            new ChallengeCornerRandom(),
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
            new ChallengeAltruismSacrifice()};
    }

    void SurvivalCriteriaManager::startNewGeneration(unsigned challengeId)
    {
        if (this->currentChallenge != nullptr)
        {
            this->currentChallenge->deleteShapes();
        }
        this->currentChallenge = this->getById(challengeId);
    }

    void SurvivalCriteriaManager::initShapes(int liveDisplayScale)
    {
        this->currentChallenge->initShapes(liveDisplayScale);
    }

    // Returns true and a score 0.0..1.0 if passed, false if failed
    std::pair<bool, float> SurvivalCriteriaManager::passedSurvivalCriterion(const Indiv &indiv, const Params &p, Grid &grid)
    {
        if (!indiv.alive)
        {
            return {false, 0.0};
        }
        return this->currentChallenge->passed(indiv, p, grid);
    }

    // Returns true and a score 0.0..1.0 if passed, false if failed
    std::pair<bool, float> SurvivalCriteriaManager::passedSurvivalCriterion(const Indiv &indiv, const Params &p, Grid &grid, unsigned challenge)
    {
        if (!indiv.alive)
        {
            return {false, 0.0};
        }
        return this->getById(challenge)->passed(indiv, p, grid);
    }

    SurvivalCriteria* SurvivalCriteriaManager::getById(unsigned id)
    {
        for (auto survivalCriteria : this->survivalCriteriasVector)
        {
            if (survivalCriteria->value == id)
            {
                return survivalCriteria;
            }
        }
        return this->survivalCriteriasVector[0];
    }

    std::vector<sf::Drawable*> SurvivalCriteriaManager::getShapes()
    {
        return this->currentChallenge->shapes;
    }
}

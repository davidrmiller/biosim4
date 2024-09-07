#include "ChallengeCornerRandom.h"

namespace BS
{
    ChallengeCornerRandom::ChallengeCornerRandom()
    {
        this->value = CHALLENGE_CORNER_RANDOM;
        this->text = "Random corners";
        this->description = "Survivors are those within the specified radius of random corners every generation. The score \
is linearly weighted by distance from the corner point";
    }

    std::pair<bool, float> ChallengeCornerRandom::passed(const Indiv &indiv, const Params &p, Grid &grid)
    {
        assert(p.sizeX == p.sizeY);
        float radius = p.sizeX / this->radiusPart;
        float distance = 0;

        std::tuple <bool, int, int> corners[4] = {
            std::make_tuple(this->cornerSurvival[0], 0, 0),
            std::make_tuple(this->cornerSurvival[1], 0, p.sizeY - 1),
            std::make_tuple(this->cornerSurvival[2], p.sizeX - 1, 0),
            std::make_tuple(this->cornerSurvival[3], p.sizeX - 1, p.sizeY - 1),
        };

        for (int i = 0; i < 4; i++)
        {
            if (std::get<0>(corners[i]))
            {
                distance = (Coord(std::get<1>(corners[i]), std::get<2>(corners[i])) - indiv.loc).length();
                if (distance <= radius)
                {
                    return {true, (radius - distance) / radius};
                }
            }
        }
        return {false, 0.0};
    }

    void ChallengeCornerRandom::initShapes(int liveDisplayScale)
    {
        float radius = p.sizeX / this->radiusPart * liveDisplayScale;
        std::fill_n(this->cornerSurvival, 4, true);
        unsigned randomCorner = std::rand() % 4;
        this->cornerSurvival[randomCorner] = false;

        std::tuple <bool, int, int> corners[4] = {
            std::make_tuple(this->cornerSurvival[0], 0 - radius, (p.sizeY * liveDisplayScale) - radius),
            std::make_tuple(this->cornerSurvival[1], 0 - radius, 0 - radius),
            std::make_tuple(this->cornerSurvival[2], (p.sizeX * liveDisplayScale) - radius, (p.sizeY * liveDisplayScale) - radius),
            std::make_tuple(this->cornerSurvival[3], (p.sizeX * liveDisplayScale) - radius, 0 - radius),
        };
        for (int i = 0; i < 4; i++)
        {
            if (std::get<0>(corners[i]))
            {
                this->createCircle(radius, std::get<1>(corners[i]), std::get<2>(corners[i]));
            }
        }

        this->createBorder(radius + 10, liveDisplayScale);
    }
}
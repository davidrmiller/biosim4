#ifndef SURVIVALCRITERIA_H_INCLUDED
#define SURVIVALCRITERIA_H_INCLUDED

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "../ai/indiv.h"
#include "../grid.h"
#include "../peeps.h"

namespace BS
{
    // Some of the survival challenges to try. Some are interesting, some
    // not so much. Fine-tune the challenges by tweaking the corresponding code
    // in survivalСriteria.cpp.
    constexpr unsigned CHALLENGE_CIRCLE = 0;
    constexpr unsigned CHALLENGE_RIGHT_HALF = 1;
    constexpr unsigned CHALLENGE_RIGHT_QUARTER = 2;
    constexpr unsigned CHALLENGE_STRING = 3;
    constexpr unsigned CHALLENGE_CENTER_WEIGHTED = 4;
    constexpr unsigned CHALLENGE_CENTER_UNWEIGHTED = 40;
    constexpr unsigned CHALLENGE_CORNER = 5;
    constexpr unsigned CHALLENGE_CORNER_WEIGHTED = 6;
    constexpr unsigned CHALLENGE_MIGRATE_DISTANCE = 7;
    constexpr unsigned CHALLENGE_CENTER_SPARSE = 8;
    constexpr unsigned CHALLENGE_LEFT_EIGHTH = 9;
    constexpr unsigned CHALLENGE_RADIOACTIVE_WALLS = 10;
    constexpr unsigned CHALLENGE_AGAINST_ANY_WALL = 11;
    constexpr unsigned CHALLENGE_TOUCH_ANY_WALL = 12;
    constexpr unsigned CHALLENGE_EAST_WEST_EIGHTHS = 13;
    constexpr unsigned CHALLENGE_NEAR_BARRIER = 14;
    constexpr unsigned CHALLENGE_PAIRS = 15;
    constexpr unsigned CHALLENGE_LOCATION_SEQUENCE = 16;
    constexpr unsigned CHALLENGE_ALTRUISM = 17;
    constexpr unsigned CHALLENGE_ALTRUISM_SACRIFICE = 18;
    constexpr unsigned CHALLENGE_CORNER_RANDOM = 19;

    class SurvivalCriteria
    {
        public:
            unsigned value;
            std::string text;
            std::string description;

            std::vector<sf::Drawable*> shapes;

            virtual std::pair<bool, float> passed(const Indiv &indiv, const Params &p, Grid &grid) = 0;
            virtual void initShapes(int liveDisplayScale);
            virtual void endOfStep(unsigned simStep, const Params &p, Grid &grid, Peeps &peeps);
            void deleteShapes();
            SurvivalCriteria(unsigned value, std::string text, std::string description) : value(value), text(text), description(description) {}
            SurvivalCriteria() {}
        protected:
            void createBorder(float size, int liveDisplayScale);
            void createCircle(float radius, float x, float y);
            void createLine(sf::Vector2f vector, sf::Vector2f vectorTwo);

            sf::Color defaultColor = sf::Color(255,255,255,127);
    };
}

#endif
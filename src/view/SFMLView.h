#ifndef SFMLVIEW_H_INCLUDED
#define SFMLVIEW_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// #include "../simulator.h"
// #include "../indiv.h"
// #include "../params.h"

namespace BS
{
    class SFMLView
    {
    public:
        SFMLView();
        ~SFMLView();

        bool isStopped();
        void updatePollEvents();
        void endOfStep();
        void endOfGeneration(unsigned generation);
    private:
        sf::RenderWindow* window;
    };
}

#endif // SFMLVIEW_H_INCLUDED
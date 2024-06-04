#ifndef SFMLUSERIO_H_INCLUDED
#define SFMLUSERIO_H_INCLUDED

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// #include "../simulator.h"
// #include "../indiv.h"
// #include "../params.h"

namespace BS
{
    class SFMLUserIO
    {
    public:
        SFMLUserIO();
        ~SFMLUserIO();

        bool isStopped();
        void updatePollEvents();
        void endOfStep();
        void endOfGeneration(unsigned generation);
        void updateInput();
        bool isPaused();

        void log(std::string message);
    private:
        int windowHeight = 900;
        int windowWidth = 1200;
        sf::RenderWindow* window;

        sf::View* view;
        tgui::Gui gui;

        tgui::TextArea::Ptr console;
        std::vector<tgui::String> logMessages;

        tgui::Picture::Ptr pausePicture;
        bool paused = false;
        void pauseResume();

        bool viewIsMoving = false;
        sf::Vector2f oldViewPos;
        double accumZoom = 1;
    };
}

#endif // SFMLUSERIO_H_INCLUDED
#ifndef SFMLUSERIO_H_INCLUDED
#define SFMLUSERIO_H_INCLUDED

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "./sfmlComponents/ViewComponent.h"
#include "./sfmlComponents/RightPanelComponent.h"
#include "./sfmlComponents/ChallengeBoxComponent.h"

namespace BS
{
    class SFMLUserIO
    {
    public:
        SFMLUserIO();
        ~SFMLUserIO();

        bool isStopped();
        bool isPaused();
        void pauseResume(bool paused);

        void updatePollEvents();
        void startNewGeneration(unsigned generation, unsigned stepsPerGeneration);
        void endOfStep(unsigned simStep);
        void endOfGeneration(unsigned generation);

        void settingsChanged(std::string name, std::string val);

        void log(std::string message);
    private:
        int windowHeight = 900;
        int windowWidth = 1200;
        sf::RenderWindow* window;

        sf::View* view;
        ViewComponent* viewComponent;
        
        tgui::Gui gui;
        RightPanelComponent* rightPanelComponent;
        bool paused = false;

        int speedThreshold = 0;
        int increaseSpeedCounter = 0;
        int slowSpeedCounter = 0;

        void speedChanged(float value);
    };
}

#endif // SFMLUSERIO_H_INCLUDED
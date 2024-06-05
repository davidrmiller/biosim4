#ifndef LEFTPANEL_H_INCLUDED
#define LEFTPANEL_H_INCLUDED

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

namespace BS
{
    class RightPanelComponent
    {
    public:
        RightPanelComponent(sf::Vector2u windowSize, std::function<void(bool)> pauseCallback);
        ~RightPanelComponent();

        void log(std::string message);
        tgui::Panel::Ptr getPanel() { return this->panel; }

        void startNewGeneration(unsigned generation, unsigned stepsPerGeneration);
        void endOfStep(unsigned simStep);
    private:
        tgui::Panel::Ptr panel;

        tgui::TextArea::Ptr console;
        std::vector<tgui::String> logMessages;

        tgui::Picture::Ptr pausePicture;
        std::function<void(bool)> pauseCallback;
        bool paused = false;
        void pauseResume();

        tgui::ProgressBar::Ptr generationProgressBar;
    };
}

#endif
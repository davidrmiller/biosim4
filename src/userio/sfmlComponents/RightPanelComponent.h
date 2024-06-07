#ifndef LEFTPANEL_H_INCLUDED
#define LEFTPANEL_H_INCLUDED

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

namespace BS
{
    class RightPanelComponent
    {
    public:
        RightPanelComponent(
            sf::Vector2u windowSize, 
            std::function<void(bool)> pauseCallback, 
            std::function<void(std::string name, std::string val)> changeSettingsCallback
        );
        ~RightPanelComponent();

        void log(std::string message);
        tgui::Panel::Ptr getPanel() { return this->panel; }

        void startNewGeneration(unsigned generation, unsigned stepsPerGeneration);
        void endOfStep(unsigned simStep);

        void addToPanel(const tgui::Widget::Ptr &widgetPtr, const tgui::String &widgetName = "");
        void initSpeedControls(int min, int max, int initValue, std::function<void(float value)> changeSpeedCallback);

    private:
        const float labelOffset = 15.f;
        const float controlOffset = 20.f;

        tgui::Panel::Ptr panel;

        tgui::TextArea::Ptr console;
        std::vector<tgui::String> logMessages;

        tgui::Picture::Ptr pausePicture;
        std::function<void(bool)> pauseCallback;
        bool paused = false;
        void pauseResume();

        tgui::ProgressBar::Ptr generationProgressBar;

        tgui::EditBox::Ptr mutationRateEditBox;

        std::function<void(std::string name, std::string val)> changeSettingsCallback;

        void createLabel(tgui::Widget::Ptr widget, const tgui::String &text);
    };
}

#endif
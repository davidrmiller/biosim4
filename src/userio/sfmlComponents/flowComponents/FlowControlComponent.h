#ifndef FLOWCONTROLCOMPONENT_H_INCLUDED
#define FLOWCONTROLCOMPONENT_H_INCLUDED

#include <functional>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "SpeedControlsComponent.h"

namespace BS
{
    /**
     * Class for displaying elements for simulation flow: pause, resume, change speed, generation progress bar.
     */
    class FlowControlComponent
    {
    public:
        FlowControlComponent(
            int speedMin, 
            int speedMax, 
            int speedInitValue, 
            std::function<void(float value)> changeSpeedCallback, 
            std::function<void(bool)> pauseCallback,
            std::function<void(bool, bool)> stopAtSmthCallback
        );
        ~FlowControlComponent();

        tgui::Group::Ptr getGroup();

        void startNewGeneration(unsigned generation, unsigned stepsPerGeneration);
        void endOfStep(unsigned simStep);

        void pauseResume();
        void pauseResume(bool paused);
        void pauseExternal(bool paused);
        void flushStopAtSmthButtons();

    private:
        const float labelOffset = 15.f;

        tgui::Group::Ptr group;

        tgui::Picture::Ptr pausePicture;
        std::function<void(bool)> pauseCallback;
        bool paused = false;
        bool externalPause = false;

        tgui::ProgressBar::Ptr generationProgressBar;

        std::function<void(bool, bool)> stopAtSmthCallback;
        tgui::ToggleButton::Ptr stopAtStartButton;
        tgui::ToggleButton::Ptr stopAtEndButton;

        void createLabel(tgui::Widget::Ptr widget, const tgui::String &text);
    };
}

#endif
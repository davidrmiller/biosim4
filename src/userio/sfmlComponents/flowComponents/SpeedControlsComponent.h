#ifndef SPEEDCONTROLSCOMPONENT_H_INCLUDED
#define SPEEDCONTROLSCOMPONENT_H_INCLUDED

#include <TGUI/TGUI.hpp>
#include <string>
#include <vector>

namespace BS
{
    class SpeedControlsComponent {
        public:
            SpeedControlsComponent(tgui::Widget::Ptr positionReferenceWidget_, tgui::Container::Ptr container_, const float controlOffset_);
            void init(int min, int max, int initValue, std::function<void(float value)> changeSpeedCallback);
            
            tgui::Widget::Ptr getLabelReferenceWidget();

        private:
            tgui::Widget::Ptr positionReferenceWidget;
            tgui::Container::Ptr container;
            float controlOffset;
            tgui::Widget::Ptr labelReferenceWidget;
    };
};

#endif
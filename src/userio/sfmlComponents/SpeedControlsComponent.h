#ifndef SPEEDCONTROLSCOMPONENT_H_INCLUDED
#define SPEEDCONTROLSCOMPONENT_H_INCLUDED

#include <TGUI/TGUI.hpp>
#include <string>
#include <vector>

namespace BS
{
    class SpeedControlsComponent {
        public:
            SpeedControlsComponent(tgui::Widget::Ptr positionReferenceWidget_, tgui::Panel::Ptr panel_, const float controlOffset_);
            void init(int min, int max, int initValue, std::function<void(float value)> changeSpeedCallback);
            
            tgui::Widget::Ptr getLabelReferenceWidget();

        private:
            tgui::Widget::Ptr positionReferenceWidget;
            tgui::Panel::Ptr panel;
            float controlOffset;
            tgui::Widget::Ptr labelReferenceWidget;
    };
};

#endif
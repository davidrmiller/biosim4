#ifndef LEFTPANEL_H_INCLUDED
#define LEFTPANEL_H_INCLUDED

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "./settingsComponents/ChallengeBoxComponent.h"
#include "./settingsComponents/BarrierBoxComponent.h"

namespace BS
{
    class RightPanelComponent
    {
    public:
        RightPanelComponent(
            sf::Vector2u windowSize, 
            std::function<void(std::string name, std::string val)> changeSettingsCallback
        );
        ~RightPanelComponent();

        tgui::Panel::Ptr getPanel() { return this->panel; }

        void addToPanel(const tgui::Widget::Ptr &widgetPtr, const tgui::String &widgetName = "");
        void initSettingsComponents();

        void setFromParams();

    private:
        const float labelOffset = 15.f;
        const float controlOffset = 20.f;

        tgui::Panel::Ptr panel;

        ChallengeBoxComponent *challengeBoxComponent;
        BarrierBoxComponent *barrierBoxComponent;
        tgui::EditBox::Ptr mutationRateEditBox;

        std::function<void(std::string name, std::string val)> changeSettingsCallback;

        void createLabel(tgui::Widget::Ptr widget, const tgui::String &text);
    };
}

#endif
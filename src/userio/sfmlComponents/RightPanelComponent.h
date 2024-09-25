#ifndef LEFTPANEL_H_INCLUDED
#define LEFTPANEL_H_INCLUDED

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "./settingsComponents/ChallengeBoxComponent.h"
#include "./settingsComponents/BarrierBoxComponent.h"

namespace BS
{
    /**
     * Container to the right of the window.
     * Directly includes settigs components, 
     * and indirectly - everything else on the right (via addToPanel function)
     */
    class RightPanelComponent
    {
    public:
        RightPanelComponent(
            sf::Vector2u windowSize, 
            std::function<void(std::string name, std::string val)> changeSettingsCallback,
            std::function<void()> infoCallback
        );
        ~RightPanelComponent();

        tgui::Panel::Ptr getPanel() { return this->panel; }

        void addToPanel(const tgui::Widget::Ptr &widgetPtr, const tgui::String &widgetName = "");
        void initSettingsComponents(std::function<void()> infoCallback);

        void setFromParams();

    private:
        const float labelOffset = 15.f;
        const float controlOffset = 20.f;

        tgui::Panel::Ptr panel;

        ChallengeBoxComponent *challengeBoxComponent;
        BarrierBoxComponent *barrierBoxComponent;
        tgui::EditBox::Ptr mutationRateEditBox;
        tgui::EditBox::Ptr populationEditBox;
        tgui::EditBox::Ptr stepsPerGenerationEditBox;

        std::function<void(std::string name, std::string val)> changeSettingsCallback;

        tgui::EditBox::Ptr createEditBox(tgui::Widget::Ptr reference, tgui::String text, std::string name);
        void createLabel(tgui::Widget::Ptr widget, const tgui::String &text);
        void createConfirmButton(tgui::EditBox::Ptr editBox, std::string settingsName, std::string name);
    };
}

#endif
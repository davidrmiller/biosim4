#ifndef BOTTOMBUTTONSCOMPONENT_H_INCLUDED
#define BOTTOMBUTTONSCOMPONENT_H_INCLUDED

#include <TGUI/TGUI.hpp>
#include <string>
#include <vector>

namespace BS
{
    class BottomButtonsComponent
    {
    public:
        BottomButtonsComponent(std::function<void(void)> saveCallback,
            std::function<void(void)> loadCallback,
            std::function<void(bool)> restartCallback,
            std::function<void()> saveIndivCallback,
            std::function<void(std::string name, std::string val)> changeSettingsCallback,
            std::function<void()> indivInfoCallback);
            
        void flushRestartButton();
        tgui::Group::Ptr getGroup();

    private:
        tgui::Group::Ptr group;
        tgui::ToggleButton::Ptr restartButton;
    };
}
#endif
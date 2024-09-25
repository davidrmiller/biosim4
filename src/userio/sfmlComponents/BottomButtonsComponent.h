#ifndef BOTTOMBUTTONSCOMPONENT_H_INCLUDED
#define BOTTOMBUTTONSCOMPONENT_H_INCLUDED

#include <TGUI/TGUI.hpp>
#include <string>
#include <vector>

namespace BS
{
    /**
     * Class for displaying buttons for interacting with simulation: save, load, restart, etc.
     */
    class BottomButtonsComponent
    {
    public:
        BottomButtonsComponent(std::function<void(void)> saveCallback,
            std::function<void(void)> loadCallback,
            std::function<void(bool)> restartCallback,
            std::function<void()> saveIndivCallback,
            std::function<void(std::string name, std::string val)> changeSettingsCallback,
            std::function<void()> indivInfoCallback,            
            std::function<void(bool)> selectPassedCallback);
            
        void flushRestartButton();
        tgui::Group::Ptr getGroup();
        void switchPassedSelection(bool selected);

    private:
        tgui::Group::Ptr group;
        tgui::ToggleButton::Ptr restartButton;
        tgui::Button::Ptr selectPassedBtn;
        bool isSelectPassed = false;
        std::function<void(bool)> selectPassedCallback;
    };
}
#endif
#ifndef CHALLENGEBOXCOMPONENT_H_INCLUDED
#define CHALLENGEBOXCOMPONENT_H_INCLUDED

#include <TGUI/TGUI.hpp>
#include <string>
#include <vector>

namespace BS
{
    struct ChallengeBoxItem
    {
        int index;
        unsigned value;
        std::string text;
        ChallengeBoxItem(){}
        ChallengeBoxItem(int index, unsigned value, std::string text) : index(index), value(value), text(text) {}
    };

    class ChallengeBoxComponent
    {
    public:
        ChallengeBoxComponent(std::function<void(std::string, std::string)> settingsChange);
        ~ChallengeBoxComponent();

        tgui::ComboBox::Ptr getChallengeBox();

        void setFromParams();
    private:
        tgui::ComboBox::Ptr challengeBox;

        std::vector<ChallengeBoxItem> items;
        int findIndexByValue(unsigned value);
        unsigned findValueByIndex(int index);

        std::function<void(std::string, std::string)> settingsChangeCallback;
    };
}

#endif
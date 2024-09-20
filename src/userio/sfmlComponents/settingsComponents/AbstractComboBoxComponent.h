#ifndef ABSTRACTCOMBOBOXCOMPONENT_H_INCLUDED
#define ABSTRACTCOMBOBOXCOMPONENT_H_INCLUDED

#include "ComboBoxItems.h"
#include <TGUI/TGUI.hpp>
#include <string>
#include <vector>

namespace BS
{
    /**
     * Base class of settings combo box
     */
    class AbstractComboBoxComponent
    {
    public:
        AbstractComboBoxComponent(std::function<void(std::string, std::string)> settingsChange);
        ~AbstractComboBoxComponent() { this->items.clear(); };
        void init();

        tgui::ComboBox::Ptr getComboBox();

        virtual void setFromParams() {};
    protected:
        virtual void initItems() {};
        virtual void onItemSelect(int index) {};

        tgui::ComboBox::Ptr comboBox;

        std::vector<ComboBoxItem> items;
        int findIndexByValue(unsigned value);
        unsigned findValueByIndex(int index);

        std::function<void(std::string, std::string)> settingsChangeCallback;
    };
}

#endif
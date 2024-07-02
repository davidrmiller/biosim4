#include "AbstractComboBoxComponent.h"

namespace BS
{
    AbstractComboBoxComponent::AbstractComboBoxComponent(std::function<void(std::string, std::string)> settingsChange)
    {
        this->settingsChangeCallback = settingsChange;

    }
    void AbstractComboBoxComponent::init()
    {
        this->initItems();

        // setup edit box
        this->comboBox = tgui::ComboBox::create();
        for (long unsigned int i = 0; i < this->items.size(); ++i)
        {
            this->comboBox->addItem(this->items[i].text);
        }
        this->setFromParams();
        this->comboBox->onItemSelect([this](int index) {
            this->onItemSelect(index);
        });
    }
    
    int AbstractComboBoxComponent::findIndexByValue(unsigned value) {
        for (long unsigned int i = 0; i < this->items.size(); ++i) {
            if (this->items[i].value == value) {
                return this->items[i].index;
            }
        }
        return this->items[0].index;
    }

    unsigned AbstractComboBoxComponent::findValueByIndex(int index) {
        for (long unsigned int i = 0; i < this->items.size(); ++i) {
            if (this->items[i].index == index) {
                return this->items[i].value;
            }
        }
        return this->items[0].value;
    }

    tgui::ComboBox::Ptr AbstractComboBoxComponent::getComboBox()
    {
        return this->comboBox;
    }
}
#include "BarrierBoxComponent.h"
#include "../../simulator.h"

namespace BS
{
    BarrierBoxComponent::BarrierBoxComponent(std::function<void(std::string, std::string)> settingsChange)
            : AbstractComboBoxComponent(settingsChange) 
    {
        this->init();
    }

    void BarrierBoxComponent::initItems()
    {
        this->items = {
            ComboBoxItem(0, 0, "No barrier"),
            ComboBoxItem(1, 1, "Vertical const"),
            ComboBoxItem(2, 2, "Vertical rand"),
            ComboBoxItem(3, 3, "5 blocks staggered"),
            ComboBoxItem(4, 4, "Horizontal const"),
            ComboBoxItem(5, 5, "3 floating islands"),
            ComboBoxItem(6, 6, "Spots, specified number, radius, locations")
        };
    }

    void BarrierBoxComponent::onItemSelect(int index)
    {
        this->settingsChangeCallback("barriertype", std::to_string(this->findValueByIndex(index)));
    }

    void BarrierBoxComponent::setFromParams()
    {
        this->comboBox->setSelectedItemByIndex(this->findIndexByValue(p.barrierType));
    }
}
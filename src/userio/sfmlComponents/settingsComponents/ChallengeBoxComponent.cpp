#include "ChallengeBoxComponent.h"
#include "../../../simulator.h"

namespace BS
{
    ChallengeBoxComponent::ChallengeBoxComponent(std::function<void(std::string, std::string)> settingsChange)
            : AbstractComboBoxComponent(settingsChange) 
    {
        this->init();
    }

    void ChallengeBoxComponent::initItems()
    {
        for (unsigned i = 0; i < survivalCriteriaManager.survivalCriteriasVector.size(); ++i)
        {
            this->items.push_back(ComboBoxItem(i, survivalCriteriaManager.survivalCriteriasVector[i]->value, survivalCriteriaManager.survivalCriteriasVector[i]->text));
        }
    }

    void ChallengeBoxComponent::onItemSelect(int index)
    {
        this->settingsChangeCallback("challenge", std::to_string(this->findValueByIndex(index)));
    }

    void ChallengeBoxComponent::setFromParams()
    {
        this->comboBox->setSelectedItemByIndex(this->findIndexByValue(p.challenge));
    }
}
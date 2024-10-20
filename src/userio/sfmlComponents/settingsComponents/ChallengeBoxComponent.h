#ifndef CHALLENGEBOXCOMPONENT_H_INCLUDED
#define CHALLENGEBOXCOMPONENT_H_INCLUDED

#include "AbstractComboBoxComponent.h"
#include "../../../survivalCriteria/SurvivalCriteria.h"
#include <TGUI/TGUI.hpp>
#include <string>
#include <vector>

namespace BS
{
    /**
     * Class for displaying challenge (survival criterias) settings
     */
    class ChallengeBoxComponent : public AbstractComboBoxComponent
    {
    public:
        ChallengeBoxComponent(std::function<void(std::string, std::string)> settingsChange);
        void setFromParams() override;
    private:
        void initItems() override;
        void onItemSelect(int index) override;
    };
}

#endif
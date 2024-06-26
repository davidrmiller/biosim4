#include "ChallengeBoxComponent.h"
#include "../../simulator.h"

namespace BS
{
    ChallengeBoxComponent::ChallengeBoxComponent(std::function<void(std::string, std::string)> settingsChange)
            : AbstractComboBoxComponent(settingsChange) 
    {
        this->init();
    }

    void ChallengeBoxComponent::initItems()
    {
        this->items = {
            ComboBoxItem(0, CHALLENGE_CIRCLE, "Circle"),
            ComboBoxItem(1, CHALLENGE_RIGHT_HALF, "Right half"),
            ComboBoxItem(2, CHALLENGE_RIGHT_QUARTER, "Right quarter"),
            ComboBoxItem(3, CHALLENGE_STRING, "String"),
            ComboBoxItem(4, CHALLENGE_CENTER_WEIGHTED, "Center weighted"),
            ComboBoxItem(5, CHALLENGE_CENTER_UNWEIGHTED, "Center unweighted"),
            ComboBoxItem(6, CHALLENGE_CORNER, "Corner"),
            ComboBoxItem(7, CHALLENGE_CORNER_WEIGHTED, "Corner weighted"),
            ComboBoxItem(8, CHALLENGE_MIGRATE_DISTANCE, "Migrate distance"),
            ComboBoxItem(9, CHALLENGE_CENTER_SPARSE, "Center sparse"),
            ComboBoxItem(10, CHALLENGE_LEFT_EIGHTH, "Left eight"),
            ComboBoxItem(11, CHALLENGE_RADIOACTIVE_WALLS, "Radioactive walls"),
            ComboBoxItem(12, CHALLENGE_AGAINST_ANY_WALL, "Against any wall"),
            ComboBoxItem(13, CHALLENGE_TOUCH_ANY_WALL, "Touch any wall"),
            ComboBoxItem(14, CHALLENGE_EAST_WEST_EIGHTHS, "East west eights"),
            ComboBoxItem(15, CHALLENGE_NEAR_BARRIER, "Near barrier"),
            ComboBoxItem(16, CHALLENGE_PAIRS, "Pairs"),
            ComboBoxItem(17, CHALLENGE_LOCATION_SEQUENCE, "Location sequence"),
            ComboBoxItem(18, CHALLENGE_ALTRUISM, "Alruism"),
            ComboBoxItem(19, CHALLENGE_ALTRUISM_SACRIFICE, "Alruism sacrifice"),
        };
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
#include "ChallengeBoxComponent.h"
#include "../../simulator.h"

namespace BS
{
    ChallengeBoxComponent::ChallengeBoxComponent(std::function<void(std::string, std::string)> settingsChange)
    {
        this->settingsChangeCallback = settingsChange;

        this->items = {
            ChallengeBoxItem(0, CHALLENGE_CIRCLE, "Circle"),
            ChallengeBoxItem(1, CHALLENGE_RIGHT_HALF, "Right half"),
            ChallengeBoxItem(2, CHALLENGE_RIGHT_QUARTER, "Right quarter"),
            ChallengeBoxItem(3, CHALLENGE_STRING, "String"),
            ChallengeBoxItem(4, CHALLENGE_CENTER_WEIGHTED, "Center weighted"),
            ChallengeBoxItem(5, CHALLENGE_CENTER_UNWEIGHTED, "Center unweighted"),
            ChallengeBoxItem(6, CHALLENGE_CORNER, "Corner"),
            ChallengeBoxItem(7, CHALLENGE_CORNER_WEIGHTED, "Corner weighted"),
            ChallengeBoxItem(8, CHALLENGE_MIGRATE_DISTANCE, "Migrate distance"),
            ChallengeBoxItem(9, CHALLENGE_CENTER_SPARSE, "Center sparse"),
            ChallengeBoxItem(10, CHALLENGE_LEFT_EIGHTH, "Left eight"),
            ChallengeBoxItem(11, CHALLENGE_RADIOACTIVE_WALLS, "Radioactive walls"),
            ChallengeBoxItem(12, CHALLENGE_AGAINST_ANY_WALL, "Against any wall"),
            ChallengeBoxItem(13, CHALLENGE_TOUCH_ANY_WALL, "Touch any wall"),
            ChallengeBoxItem(14, CHALLENGE_EAST_WEST_EIGHTHS, "East west eights"),
            ChallengeBoxItem(15, CHALLENGE_NEAR_BARRIER, "Near barrier"),
            ChallengeBoxItem(16, CHALLENGE_PAIRS, "Pairs"),
            ChallengeBoxItem(17, CHALLENGE_LOCATION_SEQUENCE, "Location sequence"),
            ChallengeBoxItem(18, CHALLENGE_ALTRUISM, "Alruism"),
            ChallengeBoxItem(19, CHALLENGE_ALTRUISM_SACRIFICE, "Alruism sacrifice"),
        };

        // setup edit box
        this->challengeBox = tgui::ComboBox::create();
        for (long unsigned int i = 0; i < this->items.size(); ++i)
        {
            this->challengeBox->addItem(this->items[i].text);
        }
        this->challengeBox->setSelectedItemByIndex(this->findIndexByValue(p.challenge));
        this->challengeBox->onItemSelect([this](int index) {
            this->settingsChangeCallback("challenge", std::to_string(this->findValueByIndex(index)));
        });
    }

    ChallengeBoxComponent::~ChallengeBoxComponent()
    {

    }

    int ChallengeBoxComponent::findIndexByValue(unsigned value) {
        for (long unsigned int i = 0; i < this->items.size(); ++i) {
            if (this->items[i].value == value) {
                return this->items[i].index;
            }
        }
        return this->items[0].index;
    }

    unsigned ChallengeBoxComponent::findValueByIndex(int index) {
        for (long unsigned int i = 0; i < this->items.size(); ++i) {
            if (this->items[i].index == index) {
                return this->items[i].value;
            }
        }
        return this->items[0].value;
    }

    tgui::ComboBox::Ptr ChallengeBoxComponent::getChallengeBox()
    {
        return this->challengeBox;
    }
}
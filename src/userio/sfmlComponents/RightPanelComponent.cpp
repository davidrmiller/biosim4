#include "RightPanelComponent.h"
#include "../../simulator.h"

namespace BS
{
    struct challengeBoxItem
    {
        int index;
        unsigned value;
        std::string text;
        challengeBoxItem(int index, unsigned value, std::string text) : index(index), value(value), text(text) {}
    };

    struct ChallengeBoxValueContainer {
        challengeBoxItem challengeBoxValues[20] = {
            challengeBoxItem(0, CHALLENGE_CIRCLE, "Circle"),
            challengeBoxItem(1, CHALLENGE_RIGHT_HALF, "Right half"),
            challengeBoxItem(2, CHALLENGE_RIGHT_QUARTER, "Right quarter"),
            challengeBoxItem(3, CHALLENGE_STRING, "String"),
            challengeBoxItem(4, CHALLENGE_CENTER_WEIGHTED, "Center weighted"),
            challengeBoxItem(5, CHALLENGE_CENTER_UNWEIGHTED, "Center unweighted"),
            challengeBoxItem(6, CHALLENGE_CORNER, "Corner"),
            challengeBoxItem(7, CHALLENGE_CORNER_WEIGHTED, "Corner weighted"),
            challengeBoxItem(8, CHALLENGE_MIGRATE_DISTANCE, "Migrate distance"),
            challengeBoxItem(9, CHALLENGE_CENTER_SPARSE, "Center sparse"),
            challengeBoxItem(10, CHALLENGE_LEFT_EIGHTH, "Left eight"),
            challengeBoxItem(11, CHALLENGE_RADIOACTIVE_WALLS, "Radioactive walls"),
            challengeBoxItem(12, CHALLENGE_AGAINST_ANY_WALL, "Against any wall"),
            challengeBoxItem(13, CHALLENGE_TOUCH_ANY_WALL, "Touch any wall"),
            challengeBoxItem(14, CHALLENGE_EAST_WEST_EIGHTHS, "East west eights"),
            challengeBoxItem(15, CHALLENGE_NEAR_BARRIER, "Near barrier"),
            challengeBoxItem(16, CHALLENGE_PAIRS, "Pairs"),
            challengeBoxItem(17, CHALLENGE_LOCATION_SEQUENCE, "Location sequence"),
            challengeBoxItem(18, CHALLENGE_ALTRUISM, "Alruism"),
            challengeBoxItem(19, CHALLENGE_ALTRUISM_SACRIFICE, "Alruism sacrifice"),
        };

        int findIndexByValue(unsigned value) {
            for (int i = 0; i < 20; ++i) {
                if (challengeBoxValues[i].value == value) {
                    return challengeBoxValues[i].index;
                }
            }
            return challengeBoxValues[0].index;
        }

        unsigned findValueByIndex(int index) {
            for (int i = 0; i < 20; ++i) {
                if (challengeBoxValues[i].index == index) {
                    return challengeBoxValues[i].value;
                }
            }
            return challengeBoxValues[0].value;
        }
    };

    ChallengeBoxValueContainer challengeBoxValueContainer;

    RightPanelComponent::RightPanelComponent(sf::Vector2u windowSize, std::function<void(bool)> pauseCallback)
    {
        this->pauseCallback = pauseCallback;

        // create panel
        this->panel = tgui::Panel::create();
        this->panel->setSize("20%", windowSize.y);
        this->panel->setAutoLayout(tgui::AutoLayout::Right);

        // setup console
        this->console = tgui::TextArea::create();
        this->console->setReadOnly(true);
        this->console->setSize("100%", "50%");
        this->console->setPosition("0%", "50%");
        this->panel->add(this->console);

        this->pausePicture = tgui::Picture::create("Resources/Pictures/play.png");
        this->pausePicture->onClick([this]()
                              { this->pauseResume(); });
        this->pausePicture->setPosition("0%", 0);
        this->panel->add(this->pausePicture, "Picture");

        // setup pause button
        tgui::Button::Ptr button = tgui::Button::create("Pause/Resume");
        button->setPosition({bindRight(this->pausePicture) + 5.f, bindTop(this->pausePicture) + 6.f});
        button->onPress([this]()
                        { this->pauseResume(); });
        this->panel->add(button, "Pause/Resume");

        // setup edit box
        tgui::ComboBox::Ptr challengeBox = tgui::ComboBox::create();
        challengeBox->setPosition("20%", 150);
        int size_of_array = sizeof(challengeBoxValueContainer.challengeBoxValues) / sizeof(challengeBoxItem);
        for (int i = 0; i < size_of_array; ++i)
        {
            challengeBox->addItem(challengeBoxValueContainer.challengeBoxValues[i].text);
        }
        challengeBox->setSelectedItemByIndex(challengeBoxValueContainer.findIndexByValue(p.challenge));
        challengeBox->onItemSelect([this](int index) {
            std::cout << "Selected: " << challengeBoxValueContainer.findValueByIndex(index) << std::endl;
        });
        this->panel->add(challengeBox, "MyEditBox");

        // setup generation progress bar
        this->generationProgressBar = tgui::ProgressBar::create();
        this->generationProgressBar->setMinimum(0);
        this->generationProgressBar->setPosition({bindLeft(pausePicture) + 5.f, bindBottom(pausePicture) + 12.f});
        this->panel->add(this->generationProgressBar, "GenerationProgressBar");
    }

    RightPanelComponent::~RightPanelComponent()
    {
    }

    void RightPanelComponent::pauseResume()
    {
        this->paused = !this->paused;

        if (this->paused)
            this->pausePicture->getRenderer()->setTexture("Resources/Pictures/pause.png");
        else
            this->pausePicture->getRenderer()->setTexture("Resources/Pictures/play.png");

        this->pauseCallback(this->paused);
    }

    void RightPanelComponent::log(std::string message)
    {
        this->logMessages.push_back(tgui::String(message));
        if (this->logMessages.size() > 100)
        {
            this->logMessages.erase(this->logMessages.begin());
        }
        this->console->setText(tgui::String::join(this->logMessages, "\n"));
    }

    void RightPanelComponent::startNewGeneration(unsigned generation, unsigned stepsPerGeneration)
    {
        this->generationProgressBar->setMaximum(stepsPerGeneration);
    }

    void RightPanelComponent::endOfStep(unsigned simStep)
    {
        this->generationProgressBar->setValue(simStep);
    }

    void RightPanelComponent::addToPanel(const tgui::Widget::Ptr &widgetPtr, const tgui::String &widgetName)
    {
        this->panel->add(widgetPtr, widgetName);
    }
}
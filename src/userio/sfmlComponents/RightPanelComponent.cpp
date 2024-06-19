#include "RightPanelComponent.h"
#include "../../simulator.h"

namespace BS
{
    RightPanelComponent::RightPanelComponent(
        sf::Vector2u windowSize,
        std::function<void(bool)> pauseCallback,
        std::function<void(std::string name, std::string val)> changeSettingsCallback)
    {
        this->pauseCallback = pauseCallback;
        this->changeSettingsCallback = changeSettingsCallback;

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

        // setup pause picture
        this->pausePicture = tgui::Picture::create("Resources/Pictures/play.png");
        this->pausePicture->onClick([this]()
                                    { this->pauseResume(); });
        this->pausePicture->setPosition("0%", 0);
        this->panel->add(this->pausePicture, "Picture");

        // setup pause button
        tgui::Button::Ptr pauseResumeButton = tgui::Button::create("Pause/Resume");
        pauseResumeButton->setPosition({bindRight(this->pausePicture) + 5.f, bindTop(this->pausePicture) + 6.f});
        pauseResumeButton->onPress([this]()
                                   { this->pauseResume(); });
        this->panel->add(pauseResumeButton, "Pause/Resume");

        // setup generation progress bar
        this->generationProgressBar = tgui::ProgressBar::create();
        this->generationProgressBar->setMinimum(0);
        this->generationProgressBar->setPosition({bindLeft(pausePicture) + 5.f, bindBottom(pausePicture) + 12.f});
        this->panel->add(this->generationProgressBar, "GenerationProgressBar");

        // setup challenge box
        ChallengeBoxComponent *challengeBoxComponent = new ChallengeBoxComponent([this](std::string name, std::string val)
                                                                                 { this->changeSettingsCallback(name, val); });
        tgui::ComboBox::Ptr challengeBox = challengeBoxComponent->getChallengeBox();
        challengeBox->setPosition("5%", "10%");
        this->panel->add(challengeBox, "ChallengeBox");
        this->createLabel(challengeBox, "Challenge");

        // setup mutation rate
        this->mutationRateEditBox = tgui::EditBox::create();
        this->mutationRateEditBox->setPosition({bindLeft(challengeBox), bindBottom(challengeBox) + this->controlOffset});
        this->mutationRateEditBox->setText(tgui::String(p.pointMutationRate));
        this->panel->add(this->mutationRateEditBox);
        this->createLabel(this->mutationRateEditBox, "Mutation Rate");

        tgui::Button::Ptr mutationButton = tgui::Button::create("Ok");
        mutationButton->setPosition({bindRight(this->mutationRateEditBox) + 2.f, bindTop(this->mutationRateEditBox)});
        mutationButton->onPress([this]()
                                { this->changeSettingsCallback("pointmutationrate", this->mutationRateEditBox->getText().toStdString()); });
        mutationButton->setHeight(this->mutationRateEditBox->getSize().y);
        this->panel->add(mutationButton, "MutationButton");
    }

    void RightPanelComponent::initSaveLoadButtons(std::function<void(void)> saveCallback, std::function<void(void)> loadCallback)
    {
        float height = this->mutationRateEditBox->getSize().y;
        tgui::Button::Ptr saveButton = tgui::Button::create("Save");
        saveButton->setPosition({bindLeft(this->console) + 10.f, bindTop(this->console) - height - 10.f});
        saveButton->onPress([saveCallback]()
                            { saveCallback(); });
        saveButton->setHeight(height);
        this->panel->add(saveButton, "SaveButton");

        tgui::Button::Ptr loadButton = tgui::Button::create("Load");
        loadButton->setPosition({bindRight(saveButton) + 5.f, bindTop(saveButton)});
        loadButton->onPress([loadCallback]()
                            { loadCallback(); });
        loadButton->setHeight(height);
        this->panel->add(loadButton, "LoadButton");
    }

    /**
     * Initializes the speed controls for the right panel component.
     *
     * @param min The minimum value for the speed controls.
     * @param max The maximum value for the speed controls.
     * @param initValue The initial value for the speed controls.
     * @param changeSpeedCallback A callback function that is called when the value of the speed controls changes.
     *
     * @throws None.
     */
    void RightPanelComponent::initSpeedControls(int min, int max, int initValue, std::function<void(float value)> changeSpeedCallback)
    {
        SpeedControlsComponent *speedControlsComponent = new SpeedControlsComponent(this->mutationRateEditBox, this->panel, this->controlOffset);
        speedControlsComponent->init(min, max, initValue, changeSpeedCallback);
        this->createLabel(speedControlsComponent->getLabelReferenceWidget(), "Speed");
    }

    void RightPanelComponent::createLabel(tgui::Widget::Ptr widget, const tgui::String &text)
    {
        tgui::Label::Ptr label = tgui::Label::create(text);
        label->setPosition({bindLeft(widget), bindTop(widget) - this->labelOffset});
        this->panel->add(label);
    }

    RightPanelComponent::~RightPanelComponent()
    {
    }

    void RightPanelComponent::pauseResume()
    {
        this->pauseResume(!this->paused);
    }

    void RightPanelComponent::pauseResume(bool paused)
    {
        this->paused = paused;

        if (this->paused || this->externalPause)
            this->pausePicture->getRenderer()->setTexture("Resources/Pictures/pause.png");
        else
            this->pausePicture->getRenderer()->setTexture("Resources/Pictures/play.png");

        this->pauseCallback(this->paused || this->externalPause);
    }

    void RightPanelComponent::pauseExternal(bool paused)
    {
        this->externalPause = paused;
        this->pauseResume(this->paused);
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
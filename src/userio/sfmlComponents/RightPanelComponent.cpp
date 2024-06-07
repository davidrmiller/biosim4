#include "RightPanelComponent.h"
#include "../../simulator.h"

namespace BS
{
    RightPanelComponent::RightPanelComponent(
        sf::Vector2u windowSize, 
        std::function<void(bool)> pauseCallback,
        std::function<void(std::string name, std::string val)> changeSettingsCallback
    )
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

        this->pausePicture = tgui::Picture::create("Resources/Pictures/play.png");
        this->pausePicture->onClick([this]()
                              { this->pauseResume(); });
        this->pausePicture->setPosition("0%", 0);
        this->panel->add(this->pausePicture, "Picture");

        // setup pause button
        tgui::Button::Ptr pauseResumeButton = tgui::Button::create("Pause/Resume");
        pauseResumeButton->setPosition({bindRight(this->pausePicture) + 5.f, bindTop(this->pausePicture) + 6.f});
        pauseResumeButton->onPress([this]()
                        { this->pauseResume(); }
        );
        this->panel->add(pauseResumeButton, "Pause/Resume");

        // setup challenge box
        ChallengeBoxComponent* challengeBoxComponent = new ChallengeBoxComponent([this](std::string name, std::string val) {
            this->changeSettingsCallback(name, val);
        });
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
                        { this->changeSettingsCallback("pointmutationrate", this->mutationRateEditBox->getText().toStdString()); }
        );
        mutationButton->setHeight(this->mutationRateEditBox->getSize().y);
        this->panel->add(mutationButton, "MutationButton");

        // setup generation progress bar
        this->generationProgressBar = tgui::ProgressBar::create();
        this->generationProgressBar->setMinimum(0);
        this->generationProgressBar->setPosition({bindLeft(pausePicture) + 5.f, bindBottom(pausePicture) + 12.f});
        this->panel->add(this->generationProgressBar, "GenerationProgressBar");
    }

    void RightPanelComponent::initSpeedControls(int min, int max, int initValue, std::function<void(float value)> changeSpeedCallback)
    {
        // setup speed controls
        tgui::SpinControl::Ptr speedControls = tgui::SpinControl::create();
        speedControls->setMinimum(min);
        speedControls->setMaximum(max);
        speedControls->setStep(1);
        speedControls->setValue(0);

        speedControls->setPosition({bindLeft(this->mutationRateEditBox), bindBottom(this->mutationRateEditBox) + this->controlOffset});
        speedControls->setHeight(this->mutationRateEditBox->getSize().y);

        speedControls->onValueChange([changeSpeedCallback](float value)
        {
            changeSpeedCallback(value);
        });

        this->panel->add(speedControls, "SpeedControls");


        this->createLabel(speedControls, "Speed");
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
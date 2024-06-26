#include "FlowControlComponent.h"

namespace BS
{

    FlowControlComponent::FlowControlComponent(
        int speedMin, 
        int speedMax, 
        int speedInitValue, 
        std::function<void(float value)> changeSpeedCallback, 
        std::function<void(bool)> pauseCallback_,
        std::function<void(bool, bool)> stopAtSmthCallback_
    )
    {
        this->pauseCallback = pauseCallback_;
        this->stopAtSmthCallback = stopAtSmthCallback_;

        this->group = tgui::Group::create();
        this->group->setPosition("0%", "0%");

        // setup pause picture
        this->pausePicture = tgui::Picture::create("Resources/Pictures/play.png");
        this->pausePicture->onClick([this]()
                                    { this->pauseResume(); });
        this->pausePicture->setPosition(5.f, 5.f);
        this->group->add(this->pausePicture, "Picture");

        // create StopAtStartButton
        this->stopAtStartButton = tgui::ToggleButton::create(">|");
        this->stopAtStartButton->setPosition({bindRight(this->pausePicture) + 3.f, bindTop(this->pausePicture) + 6.f});
        this->stopAtStartButton->setWidth(20.f);
        this->stopAtStartButton->onToggle([this](bool isDown) {
            if (isDown) {
                this->stopAtSmthCallback(true, false);
                this->stopAtEndButton->setDown(false);
            }
            else {
                this->stopAtSmthCallback(false, this->stopAtEndButton->isDown());
            } 
        });
        this->group->add(this->stopAtStartButton, "StopAtStartButton");

        // setup generation progress bar
        this->generationProgressBar = tgui::ProgressBar::create();
        this->generationProgressBar->setMinimum(0);
        this->generationProgressBar->setWidth("60%");
        this->generationProgressBar->setPosition({bindRight(this->stopAtStartButton), bindTop(this->stopAtStartButton)});
        this->group->add(this->generationProgressBar, "GenerationProgressBar");

        this->stopAtEndButton = tgui::ToggleButton::create(">|");
        this->stopAtEndButton->setPosition({bindRight(this->generationProgressBar) + 3.f, bindTop(this->generationProgressBar)});
        this->stopAtEndButton->setWidth(20.f);
        this->stopAtEndButton->onToggle([this](bool isDown) {
            if (isDown) {
                this->stopAtSmthCallback(false, true);
                this->stopAtStartButton->setDown(false);
            }
            else {
                this->stopAtSmthCallback(this->stopAtEndButton->isDown(), false);
            } 
        });
        this->group->add(this->stopAtEndButton, "StopAtEndButton");

        SpeedControlsComponent *speedControlsComponent = new SpeedControlsComponent(this->pausePicture, this->group, 20.f);
        speedControlsComponent->init(speedMin, speedMax, speedInitValue, changeSpeedCallback);
        tgui::Widget::Ptr referenceWidget = speedControlsComponent->getLabelReferenceWidget();
        this->createLabel(referenceWidget, "Speed");
    }

    tgui::Group::Ptr FlowControlComponent::getGroup() { 
        return this->group; 
    }    

    FlowControlComponent::~FlowControlComponent() {}

    void FlowControlComponent::pauseResume()
    {
        this->pauseResume(!this->paused);
    }

    void FlowControlComponent::pauseResume(bool paused)
    {
        this->paused = paused;

        if (this->paused || this->externalPause)
            this->pausePicture->getRenderer()->setTexture("Resources/Pictures/pause.png");
        else
            this->pausePicture->getRenderer()->setTexture("Resources/Pictures/play.png");

        this->pauseCallback(this->paused || this->externalPause);
    }

    void FlowControlComponent::pauseExternal(bool paused)
    {
        this->externalPause = paused;
        this->pauseResume(this->paused);
    }

    void FlowControlComponent::startNewGeneration(unsigned generation, unsigned stepsPerGeneration)
    {
        this->generationProgressBar->setMaximum(stepsPerGeneration);
    }

    void FlowControlComponent::endOfStep(unsigned simStep)
    {
        this->generationProgressBar->setValue(simStep);
    }

    void FlowControlComponent::createLabel(tgui::Widget::Ptr widget, const tgui::String &text)
    {
        tgui::Label::Ptr label = tgui::Label::create(text);
        label->setPosition({bindLeft(widget), bindTop(widget) - this->labelOffset});
        this->group->add(label);
    }

    void FlowControlComponent::flushStopAtSmthButtons()
    {
        this->stopAtStartButton->setDown(false);
        this->stopAtEndButton->setDown(false);
    }    
}
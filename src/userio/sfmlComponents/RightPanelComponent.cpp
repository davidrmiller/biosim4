#include "RightPanelComponent.h"

namespace BS
{
    RightPanelComponent::RightPanelComponent(sf::Vector2u windowSize, std::function<void(bool)> pauseCallback)
    {
        this->pauseCallback = pauseCallback;

        // create panel
        this->panel = tgui::Panel::create();
        panel->setSize("20%", windowSize.y);
        panel->setAutoLayout(tgui::AutoLayout::Right);        

        // setup console
        this->console = tgui::TextArea::create();
        this->console->setReadOnly(true);
        this->console->setSize("100%", "50%");
        this->console->setPosition("0%", "50%");
        panel->add(this->console);

        pausePicture = tgui::Picture::create("Resources/Pictures/play.png");
        pausePicture->onClick([this]()
                              { this->pauseResume(); });
        pausePicture->setPosition("0%", 0);
        panel->add(pausePicture, "Picture");

        // setup pause button
        tgui::Button::Ptr button = tgui::Button::create("Pause/Resume");
        button->setPosition({bindRight(pausePicture) + 5.f, bindTop(pausePicture) + 6.f});
        button->onPress([this]()
                        { this->pauseResume(); });
        panel->add(button, "Pause/Resume");

        // setup edit box
        auto editBox = tgui::EditBox::create();
        editBox->setPosition("20%", 150);
        panel->add(editBox, "MyEditBox");

        this->generationProgressBar = tgui::ProgressBar::create();
        this->generationProgressBar->setMinimum(0);
        this->generationProgressBar->setPosition({bindLeft(pausePicture) + 5.f, bindBottom(pausePicture) + 12.f});
        panel->add(this->generationProgressBar, "GenerationProgressBar");        
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
}
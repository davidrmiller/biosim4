#include "RightPanelComponent.h"
#include "../../simulator.h"

namespace BS
{
    RightPanelComponent::RightPanelComponent(
        sf::Vector2u windowSize, 
        std::function<void(std::string name, std::string val)> changeSettingsCallback_,
        std::function<void()> infoCallback
    )
    {
        this->changeSettingsCallback = changeSettingsCallback_;

        // create panel
        this->panel = tgui::Panel::create();
        this->panel->setSize("20%", windowSize.y);
        this->panel->setAutoLayout(tgui::AutoLayout::Right);

        this->initSettingsComponents(infoCallback);
    }

    void RightPanelComponent::initSettingsComponents(std::function<void()> infoCallback)
    {
        // setup challenge box
        this->challengeBoxComponent = new ChallengeBoxComponent([this](std::string name, std::string val) { 
            this->changeSettingsCallback(name, val); 
        });
        tgui::ComboBox::Ptr challengeBox = this->challengeBoxComponent->getComboBox();
        challengeBox->setPosition("5%", "15%");
        this->panel->add(challengeBox, "ChallengeBox");
        this->createLabel(challengeBox, "Challenge");

        tgui::Button::Ptr challengeInfoBtn = tgui::Button::create("i");
        challengeInfoBtn->setPosition({bindRight(challengeBox) + 5.f, bindTop(challengeBox)});
        challengeInfoBtn->onPress([infoCallback]() { 
            infoCallback(); 
        });
        challengeInfoBtn->setHeight(challengeBox->getSize().y);
        this->panel->add(challengeInfoBtn, "IndivInfoBtn");

        // setup mutation rate
        this->mutationRateEditBox = this->createEditBox(challengeBox, tgui::String(p.pointMutationRate), "Mutation Rate");
        this->createConfirmButton(this->mutationRateEditBox, "pointmutationrate", "MutationButton");        
        
        // setup barrier box
        this->barrierBoxComponent = new BarrierBoxComponent([this](std::string name, std::string val) {
            this->changeSettingsCallback(name, val); 
        });
        tgui::ComboBox::Ptr barrierBox = this->barrierBoxComponent->getComboBox();
        barrierBox->setPosition({bindLeft(this->mutationRateEditBox), bindBottom(this->mutationRateEditBox) + this->controlOffset});
        this->panel->add(barrierBox, "BarrierBox");
        this->createLabel(barrierBox, "Barrier");

        // setup population
        this->populationEditBox = this->createEditBox(this->barrierBoxComponent->getComboBox(), tgui::String(p.population), "Population");
        this->createConfirmButton(this->populationEditBox, "population", "PopulationButton");

        // setup steps per generation
        this->stepsPerGenerationEditBox = this->createEditBox(this->populationEditBox, tgui::String(p.stepsPerGeneration), "Steps Per Generation");
        this->createConfirmButton(this->stepsPerGenerationEditBox, "stepspergeneration", "StepsPerGenerationButton");
    }

    tgui::EditBox::Ptr RightPanelComponent::createEditBox(tgui::Widget::Ptr reference, tgui::String text, std::string name)
    {
        tgui::EditBox::Ptr editBox = tgui::EditBox::create();
        editBox->setPosition({bindLeft(reference), bindBottom(reference) + this->controlOffset});
        editBox->setText(text);
        this->panel->add(editBox);
        this->createLabel(editBox, name);
        return editBox;
    }

    void RightPanelComponent::createConfirmButton(tgui::EditBox::Ptr editBox, std::string settingsName, std::string name)
    {
        tgui::Button::Ptr button = tgui::Button::create("Ok");
        button->setPosition({bindRight(editBox) + 2.f, bindTop(editBox)});
        button->onPress([this, settingsName, editBox]() { 
            this->changeSettingsCallback(settingsName, editBox->getText().toStdString()); 
        });
        button->setHeight(editBox->getSize().y);
        this->panel->add(button, name);
    }

    void RightPanelComponent::setFromParams()
    {
        this->challengeBoxComponent->setFromParams();
        this->mutationRateEditBox->setText(tgui::String(p.pointMutationRate));
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

    void RightPanelComponent::addToPanel(const tgui::Widget::Ptr &widgetPtr, const tgui::String &widgetName)
    {
        this->panel->add(widgetPtr, widgetName);
    }
}
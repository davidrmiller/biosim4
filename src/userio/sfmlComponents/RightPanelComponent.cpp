#include "RightPanelComponent.h"
#include "../../simulator.h"

namespace BS
{
    RightPanelComponent::RightPanelComponent(
        sf::Vector2u windowSize, 
        std::function<void(std::string name, std::string val)> changeSettingsCallback_
    )
    {
        this->changeSettingsCallback = changeSettingsCallback_;

        // create panel
        this->panel = tgui::Panel::create();
        this->panel->setSize("20%", windowSize.y);
        this->panel->setAutoLayout(tgui::AutoLayout::Right);

        this->initSettingsComponents();
    }

    void RightPanelComponent::initSettingsComponents()
    {
        // setup challenge box
        this->challengeBoxComponent = new ChallengeBoxComponent([this](std::string name, std::string val)
                                                                                 { this->changeSettingsCallback(name, val); });
        tgui::ComboBox::Ptr challengeBox = this->challengeBoxComponent->getComboBox();
        challengeBox->setPosition("5%", "15%");
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
        
        // setup barrier box
        this->barrierBoxComponent = new BarrierBoxComponent([this](std::string name, std::string val)
                                                                                 { this->changeSettingsCallback(name, val); });
        tgui::ComboBox::Ptr barrierBox = this->barrierBoxComponent->getComboBox();
        barrierBox->setPosition({bindLeft(this->mutationRateEditBox), bindBottom(this->mutationRateEditBox) + this->controlOffset});
        this->panel->add(barrierBox, "BarrierBox");
        this->createLabel(barrierBox, "Barrier");
    }

    void RightPanelComponent::setFromParams()
    {
        this->challengeBoxComponent->setFromParams();
        this->mutationRateEditBox->setText(tgui::String(p.pointMutationRate));
    }

    void RightPanelComponent::initBottomButtons(
        std::function<void(void)> saveCallback,
        std::function<void(void)> loadCallback, 
        std::function<void(bool)> restartCallback,
        std::function<void()> saveIndivCallback)
    {
        // setup divider
        tgui::SeparatorLine::Ptr line = tgui::SeparatorLine::create();
        line->setPosition("0%", "75%");
        line->setSize("100%", 1);
        line->getRenderer()->setColor(tgui::Color::Black);
        this->panel->add(line);

        // setup buttons
        float height = this->mutationRateEditBox->getSize().y;
        tgui::Button::Ptr saveButton = tgui::Button::create("Save sim");
        saveButton->setPosition({bindLeft(line) + 10.f, bindTop(line) - height - 10.f});
        saveButton->onPress([saveCallback]()
                            { saveCallback(); });
        saveButton->setHeight(height);
        this->panel->add(saveButton, "SaveButton");

        tgui::Button::Ptr loadButton = tgui::Button::create("Load sim");
        loadButton->setPosition({bindRight(saveButton) + 5.f, bindTop(saveButton)});
        loadButton->onPress([loadCallback]()
                            { loadCallback(); });
        loadButton->setHeight(height);
        this->panel->add(loadButton, "LoadButton");

        this->restartButton = tgui::ToggleButton::create("Restart");
        this->restartButton->setPosition({bindRight(loadButton) + 5.f, bindTop(loadButton)});
        this->restartButton->onToggle([restartCallback](bool isDown) {
            restartCallback(isDown);
        });
        this->restartButton->setHeight(height);
        this->panel->add(this->restartButton, "RestartButton");

        tgui::CheckBox::Ptr autosaveCheckBox = tgui::CheckBox::create("Autosave");
        autosaveCheckBox->setPosition({bindLeft(saveButton), bindTop(loadButton) - autosaveCheckBox->getSize().y * 1.5f});
        autosaveCheckBox->setText("Autosave");
        autosaveCheckBox->setChecked(p.autoSave);
        autosaveCheckBox->onChange([this](bool checked){
            this->changeSettingsCallback("autosave", checked ? "1" : "0");
        });
        this->panel->add(autosaveCheckBox, "AutosaveCheckBox");

        tgui::Button::Ptr tempBtn = tgui::Button::create("Save indiv");
        tempBtn->setPosition({bindRight(this->restartButton) - tempBtn->getSize().x, bindTop(this->restartButton) - tempBtn->getSize().y - 10.f});
        tempBtn->onPress([saveIndivCallback]()
                         { saveIndivCallback(); });
        tempBtn->setHeight(height);
        this->panel->add(tempBtn, "TempButton");
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

    void RightPanelComponent::flushRestartButton()
    {
        this->restartButton->setDown(false);
    }
}
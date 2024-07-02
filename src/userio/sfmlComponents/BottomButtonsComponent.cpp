#include "BottomButtonsComponent.h"
#include "../../simulator.h"

namespace BS
{
    BottomButtonsComponent::BottomButtonsComponent(std::function<void(void)> saveCallback,
        std::function<void(void)> loadCallback,
        std::function<void(bool)> restartCallback,
        std::function<void()> saveIndivCallback,
        std::function<void(std::string name, std::string val)> changeSettingsCallback)
    {
        this->group = tgui::Group::create();

        // setup divider
        tgui::SeparatorLine::Ptr line = tgui::SeparatorLine::create();
        line->setPosition("0%", "75%");
        line->setSize("100%", 1);
        line->getRenderer()->setColor(tgui::Color::Black);
        this->group->add(line);

        // setup buttons
        float height = 27.f;
        tgui::Button::Ptr saveButton = tgui::Button::create("Save sim");
        saveButton->setPosition({bindLeft(line) + 10.f, bindTop(line) - height - 10.f});
        saveButton->onPress([saveCallback]()
                            { saveCallback(); });
        saveButton->setHeight(height);
        this->group->add(saveButton, "SaveButton");

        tgui::Button::Ptr loadButton = tgui::Button::create("Load sim");
        loadButton->setPosition({bindRight(saveButton) + 5.f, bindTop(saveButton)});
        loadButton->onPress([loadCallback]()
                            { loadCallback(); });
        loadButton->setHeight(height);
        this->group->add(loadButton, "LoadButton");

        this->restartButton = tgui::ToggleButton::create("Restart");
        this->restartButton->setPosition({bindRight(loadButton) + 5.f, bindTop(loadButton)});
        this->restartButton->onToggle([restartCallback](bool isDown) {
            restartCallback(isDown);
        });
        this->restartButton->setHeight(height);
        this->group->add(this->restartButton, "RestartButton");

        tgui::CheckBox::Ptr autosaveCheckBox = tgui::CheckBox::create("Autosave");
        autosaveCheckBox->setPosition({bindLeft(saveButton), bindTop(loadButton) - autosaveCheckBox->getSize().y * 1.5f});
        autosaveCheckBox->setText("Autosave");
        autosaveCheckBox->setChecked(p.autoSave);
        autosaveCheckBox->onChange([changeSettingsCallback](bool checked){
            changeSettingsCallback("autosave", checked ? "1" : "0");
        });
        this->group->add(autosaveCheckBox, "AutosaveCheckBox");

        tgui::Button::Ptr tempBtn = tgui::Button::create("Save indiv");
        tempBtn->setPosition({bindRight(this->restartButton) - tempBtn->getSize().x, bindTop(this->restartButton) - tempBtn->getSize().y - 10.f});
        tempBtn->onPress([saveIndivCallback]()
                         { saveIndivCallback(); });
        tempBtn->setHeight(height);
        this->group->add(tempBtn, "TempButton");
    }    
    
    void BottomButtonsComponent::flushRestartButton()
    {
        this->restartButton->setDown(false);
    }

    tgui::Group::Ptr BottomButtonsComponent::getGroup() { 
        return this->group; 
    } 
}
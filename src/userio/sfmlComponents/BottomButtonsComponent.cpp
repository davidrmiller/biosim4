#include "BottomButtonsComponent.h"
#include "../../simulator.h"

namespace BS
{
    BottomButtonsComponent::BottomButtonsComponent(std::function<void(void)> saveCallback,
        std::function<void(void)> loadCallback,
        std::function<void(bool)> restartCallback,
        std::function<void()> saveIndivCallback,
        std::function<void(std::string name, std::string val)> changeSettingsCallback,
        std::function<void()> indivInfoCallback,
        std::function<void(bool)> selectPassedCallback)
    {
        this->selectPassedCallback = selectPassedCallback;

        this->group = tgui::Group::create();

        // setup divider
        tgui::SeparatorLine::Ptr line = tgui::SeparatorLine::create();
        line->setPosition("0%", "75%");
        line->setSize("100%", 1);
        line->getRenderer()->setColor(tgui::Color::Black);
        this->group->add(line);

        // setup buttons
        float btnHorizontalMargin = 5.f;
        float height = 27.f;
        tgui::Button::Ptr saveButton = tgui::Button::create("Save sim");
        saveButton->setPosition({bindLeft(line) + 10.f, bindTop(line) - height - 10.f});
        saveButton->onPress([saveCallback]() { saveCallback(); });
        saveButton->setHeight(height);
        this->group->add(saveButton, "SaveButton");

        tgui::Button::Ptr loadButton = tgui::Button::create("Load sim");
        loadButton->setPosition({bindRight(saveButton) + btnHorizontalMargin, bindTop(saveButton)});
        loadButton->onPress([loadCallback]() { loadCallback(); });
        loadButton->setHeight(height);
        this->group->add(loadButton, "LoadButton");

        this->restartButton = tgui::ToggleButton::create("Restart");
        this->restartButton->setPosition({bindRight(loadButton) + btnHorizontalMargin, bindTop(loadButton)});
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

        tgui::Button::Ptr saveIndivBtn = tgui::Button::create("Save indiv");
        saveIndivBtn->setPosition({bindRight(this->restartButton) - saveIndivBtn->getSize().x, bindTop(this->restartButton) - saveIndivBtn->getSize().y - 10.f});
        saveIndivBtn->onPress([saveIndivCallback]() { saveIndivCallback(); });
        saveIndivBtn->setHeight(height);
        this->group->add(saveIndivBtn, "SaveIndivBtn");

        tgui::Button::Ptr indivInfoBtn = tgui::Button::create("i");
        indivInfoBtn->setPosition({bindLeft(saveIndivBtn) - indivInfoBtn->getSize().x - btnHorizontalMargin, bindTop(saveIndivBtn)});
        indivInfoBtn->onPress([indivInfoCallback]() { indivInfoCallback(); });
        indivInfoBtn->setHeight(height);
        this->group->add(indivInfoBtn, "IndivInfoBtn");

        this->selectPassedBtn = tgui::Button::create("Passed");
        this->selectPassedBtn->setPosition({bindLeft(saveIndivBtn), bindTop(saveIndivBtn) - this->selectPassedBtn->getSize().y - 10.f});
        this->selectPassedBtn->onPress([this]() { 
            this->isSelectPassed = !this->isSelectPassed;
            this->selectPassedBtn->setText(this->isSelectPassed ? "Clear" : "Passed");
            this->selectPassedCallback(this->isSelectPassed); 
        });
        this->selectPassedBtn->setHeight(height);
        this->group->add(selectPassedBtn, "SelectPassedBtn");
    }    
    
    void BottomButtonsComponent::switchPassedSelection(bool selected)
    {
        this->isSelectPassed = selected;
        this->selectPassedBtn->setText(this->isSelectPassed ? "Clear" : "Passed");
        this->selectPassedCallback(this->isSelectPassed); 
    }

    void BottomButtonsComponent::flushRestartButton()
    {
        this->restartButton->setDown(false);
    }

    tgui::Group::Ptr BottomButtonsComponent::getGroup() 
    { 
        return this->group; 
    } 
}
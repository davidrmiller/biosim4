#include "InfoWindowComponent.h"

namespace BS
{
    InfoWindowComponent::InfoWindowComponent(std::function<void(void)> closeCallback)
    {
        this->childWindow = tgui::ChildWindow::create();
        this->childWindow->setSize("50%", "100%");
        this->childWindow->setPosition("25%", "0%");
        this->childWindow->setTitle("Info");
        this->childWindow->setTitleButtons(tgui::ChildWindow::TitleButton::Close);
        this->childWindow->setResizable(false);
        this->childWindow->setKeepInParent(true); // Prevents any part of the window to go outside the screen
        childWindow->onClosing([this, closeCallback](bool* abort){
            this->flush();
            closeCallback();
        });

        this->messageArea = tgui::TextArea::create();
        this->messageArea->setReadOnly(true);
        this->messageArea->setSize("100%", "100%");
        // this->console->setPosition("80%", "75%");

        this->childWindow->add(this->messageArea);
    }

    void InfoWindowComponent::append(std::string message)
    {
        // tgui::String msg = tgui::String(message);
        //this->messages.push_back(msg);
        this->messages.push_back(tgui::String(message));
        this->messageArea->setText(tgui::String::join(this->messages, "\n"));
    }

    void InfoWindowComponent::flush()
    {
        this->messages.clear();
        this->messageArea->setText("");
    }
}
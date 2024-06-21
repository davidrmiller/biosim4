#include "ConsoleComponent.h"

namespace BS
{
    ConsoleComponent::ConsoleComponent()
    {
        // setup console
        this->console = tgui::TextArea::create();
        this->console->setReadOnly(true);
        this->console->setSize("20%", "25%");
        this->console->setPosition("80%", "75%");
    }

    ConsoleComponent::~ConsoleComponent(){}

    void ConsoleComponent::log(std::string message)
    {
        this->logMessages.push_back(tgui::String(message));
        if (this->logMessages.size() > 100)
        {
            this->logMessages.erase(this->logMessages.begin());
        }
        this->console->setText(tgui::String::join(this->logMessages, "\n"));
    }
}
#ifndef CONSOLECOMPONENT_H_INCLUDED
#define CONSOLECOMPONENT_H_INCLUDED

#include <TGUI/TGUI.hpp>
#include <string>
#include <vector>

namespace BS
{
    class ConsoleComponent
    {
    public: 
        ConsoleComponent();
        ~ConsoleComponent();
        void log(std::string message);
        tgui::TextArea::Ptr getConsole() { return this->console; }
    private:
        tgui::TextArea::Ptr console;
        std::vector<tgui::String> logMessages;
    };
}

#endif

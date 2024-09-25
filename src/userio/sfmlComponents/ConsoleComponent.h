#ifndef CONSOLECOMPONENT_H_INCLUDED
#define CONSOLECOMPONENT_H_INCLUDED

#include <TGUI/TGUI.hpp>
#include <string>
#include <vector>

namespace BS
{
    /**
     * Class for displaying console messages, like generation counter, number of survivors, etc.
     */
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

#ifndef INFOWINDOWCOMPONENT_H_INCLUDED
#define INFOWINDOWCOMPONENT_H_INCLUDED

#include <TGUI/TGUI.hpp>
#include <string>
#include <vector>

namespace BS
{
    /**
     * Class represents child window with text message in it
     * Can be used to display info about challenges and indivs
     */
    class InfoWindowComponent
    {
    public:
        InfoWindowComponent(std::function<void(void)> closeCallback);
        void append(std::string message);
        void flush();
        tgui::ChildWindow::Ptr getChildWindow() { return this->childWindow; }

    private:
        tgui::ChildWindow::Ptr childWindow;
        tgui::TextArea::Ptr messageArea;
        std::vector<tgui::String> messages;
    };
}

#endif

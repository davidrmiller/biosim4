#ifndef SFMLUSERIO_H_INCLUDED
#define SFMLUSERIO_H_INCLUDED

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../ai/indiv.h"
#include "../params.h"
#include "../peeps.h"
#include "../utils/Save.h"

#include "./sfmlComponents/ViewComponent.h"
#include "./sfmlComponents/RightPanelComponent.h"
#include "./sfmlComponents/FlowControlComponent.h"
#include "./sfmlComponents/ConsoleComponent.h"

namespace BS
{
    extern Peeps peeps;
    extern const Params &p;
    extern ParamManager paramManager;
    
    class SFMLUserIO
    {
    public:
        SFMLUserIO();
        ~SFMLUserIO();

        bool isStopped();
        bool isPaused();
        void pauseResume(bool paused);

        void updatePollEvents();
        void startNewGeneration(unsigned generation, unsigned stepsPerGeneration);
        void endOfStep(unsigned simStep);
        void endOfGeneration(unsigned generation);

        void settingsChanged(std::string name, std::string val);

        void log(std::string message);

        bool loadFileSelected = false;
        std::string loadFilename;

        void setFromParams();
    private:
        static int const SPEED_SLOW_MAX = -5;
        static int const SPEED_FAST_MAX = 5;

        int windowHeight = 900;
        int windowWidth = 1200;
        sf::RenderWindow* window;

        sf::View* view;
        ViewComponent* viewComponent;
        
        tgui::Gui gui;
        RightPanelComponent* rightPanelComponent;
        FlowControlComponent* flowControlComponent;
        bool paused = false;
        ConsoleComponent* console;

        int speedThreshold = 0;
        int increaseSpeedCounter = 0;
        int slowSpeedCounter = 0;

        void speedChanged(float value);

        bool isFileDialogShowing = false;
        tgui::FileDialog::Ptr loadFileDialog;
        tgui::FileDialog::Ptr saveFileDialog;
        void fileDialogToggled(bool shown);

        bool stopAtEnd = false;
        bool stopAtStart = false;
    };
}

#endif // SFMLUSERIO_H_INCLUDED
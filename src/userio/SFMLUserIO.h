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
#include "../survivalCriteria/SurvivalCriteriaManager.h"

#include "./sfmlComponents/ViewComponent.h"
#include "./sfmlComponents/RightPanelComponent.h"
#include "./sfmlComponents/flowComponents/FlowControlComponent.h"
#include "./sfmlComponents/ConsoleComponent.h"
#include "./sfmlComponents/BottomButtonsComponent.h"
#include "./sfmlComponents/InfoWindowComponent.h"

namespace BS
{
    extern Peeps peeps;
    extern Grid grid;
    extern const Params &p;
    extern ParamManager paramManager;
    extern SurvivalCriteriaManager survivalCriteriaManager;
    extern std::stringstream printGenomeLegend();
    
    /**
     * This class displays of a simulation using the SFML library.
     * Main display loop and event handling are done in this class.
     * Includes window, view, all custom display components and child dialogs.
     */
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

        bool restartOnEnd = false;
    private:
        int windowHeight = 900;
        int windowWidth = 1200;
        sf::RenderWindow* window;

        sf::View* view;
        ViewComponent* viewComponent;
        
        tgui::Gui gui;
        RightPanelComponent* rightPanelComponent;
        FlowControlComponent* flowControlComponent;
        BottomButtonsComponent* bottomButtonsComponent;
        InfoWindowComponent* infoWindowComponent;

        bool paused = false;
        ConsoleComponent* console;

        static int const SPEED_SLOW_MAX = -5;   // max speed factor
        static int const SPEED_FAST_MAX = 5;    // min speed factor
        int speedThreshold = 0;         // how many frames should be skipped (positive value) or drawn again (negative value)
        int increaseSpeedCounter = 0;   // how many frames were skipped and not drawn
        int slowSpeedCounter = 0;       // how many times were current frame drawn
        void speedChanged(float value);

        bool isChildWindowShowing = false;
        tgui::FileDialog::Ptr loadFileDialog;
        tgui::FileDialog::Ptr saveFileDialog;
        void childWindowToggled(bool shown);
        void initSaveFileDialog();
        void initLoadFileDialog();

        bool stopAtEnd = false;
        bool stopAtStart = false;

        std::vector<sf::RectangleShape> barriesrs;

        int getLiveDisplayScale();
        uint16_t selectedIndex = 0; // selected individual index

        bool passedSelected = false;
    };
}

#endif // SFMLUSERIO_H_INCLUDED
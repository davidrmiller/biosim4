#include "SFMLUserIO.h"

namespace BS
{
    /**
     * Initiates all components
     * Custom components are created using callbacks
     */
    SFMLUserIO::SFMLUserIO()
    {
        this->window = new sf::RenderWindow(sf::VideoMode(this->windowWidth, this->windowHeight), "biosim4", sf::Style::Close | sf::Style::Titlebar);
        this->window->setFramerateLimit(144);
        this->window->setVerticalSyncEnabled(false);
        this->window->setPosition(sf::Vector2i(200, (sf::VideoMode::getDesktopMode().height - this->windowHeight) / 2));

        // setup gui
        this->gui.setWindow(*this->window);
        tgui::Theme::setDefault("./Resources/Black.txt");

        // setup right panel component
        this->rightPanelComponent = new RightPanelComponent(
            this->window->getSize(),
            [this](std::string name, std::string val) // changeSettingsCallback
            {
                this->settingsChanged(name, val);
            },            
            [this]() // infoCallback
            {
                if (this->isChildWindowShowing)
                {
                    return;
                }
                for (unsigned i = 0; i < survivalCriteriaManager.survivalCriteriasVector.size(); ++i)
                {
                    this->infoWindowComponent->append(survivalCriteriaManager.survivalCriteriasVector[i]->text + ": \n" + survivalCriteriaManager.survivalCriteriasVector[i]->description + "\n");
                }
                this->childWindowToggled(true);
                this->gui.add(this->infoWindowComponent->getChildWindow());
            });

        // setup flow control component
        this->flowControlComponent = new FlowControlComponent(
            SPEED_SLOW_MAX,
            SPEED_FAST_MAX,
            0,
            [this](float value) // changeSpeedCallback
            {
                this->speedChanged(value);
            },
            [this](bool paused) // pauseCallback_
            {
                this->pauseResume(paused);
            },
            [this](bool stopAtStart, bool stopAtEnd) // stop at start or end callback
            {
                this->stopAtEnd = stopAtEnd; 
                this->stopAtStart = stopAtStart;
            }
        );
        
        // setup info component
        this->infoWindowComponent = new InfoWindowComponent([this]{
            this->childWindowToggled(false);
        });

        // setup bottom buttons component
        this->bottomButtonsComponent = new BottomButtonsComponent(
            [this](void) // saveCallback
            {
                if (this->isChildWindowShowing)
                {
                    return;
                }
                this->initSaveFileDialog();

                this->saveFileDialog->setPath("Output/Saves"); // update files list
                this->childWindowToggled(true);
                this->gui.add(this->saveFileDialog);
            },
            [this]() // loadCallback
            {
                if (this->isChildWindowShowing)
                {
                    return;
                }

                this->initLoadFileDialog();

                this->loadFileDialog->setPath("Output/Saves"); // update files list
                this->childWindowToggled(true);
                this->gui.add(this->loadFileDialog);
            },
            [this](bool restart) // restartCallback
            {
                this->restartOnEnd = restart;
            },
            [this]() // saveIndivCallback
            {
                this->flowControlComponent->pauseResume(true);
                
                if (this->selectedIndex != 0) {
                    std::string filename = Save::saveNet(this->selectedIndex);                    
                    this->log("Saved into: " + filename);
                }
            },
            [this](std::string name, std::string val) // changeSettingsCallback
            {
                this->settingsChanged(name, val);
            },
            [this]() // indivInfoCallback
            {
                if (this->isChildWindowShowing)
                {
                    return;
                }
                std::stringstream legendStream = printGenomeLegend();
                this->infoWindowComponent->append(legendStream.str());                
                if (this->selectedIndex != 0) 
                {
                    std::stringstream indivStream = peeps[this->selectedIndex].printNeuralNet();
                    this->infoWindowComponent->append(indivStream.str());   
                    this->infoWindowComponent->append("\nLocation: " + 
                        std::to_string(peeps[this->selectedIndex].loc.x) + ", " + 
                        std::to_string(peeps[this->selectedIndex].loc.y));
                }
                this->childWindowToggled(true);
                this->gui.add(this->infoWindowComponent->getChildWindow());
            },
            [this](bool select) // selectPassedCallback
            {
                if (select) {
                    this->passedSelected = true;
                    peeps[this->selectedIndex].shape.setOutlineThickness(0.f);
                    this->selectedIndex = 0;

                    for (uint16_t index = 1; index <= p.population; ++index) {
                        std::pair<bool, float> passed = survivalCriteriaManager.passedSurvivalCriterion(peeps[index], p, grid);
                        if (passed.first) {  
                            Indiv peep = peeps[index];                      
                            peeps[index].shape.setOutlineColor(sf::Color::White);
                            peeps[index].shape.setOutlineThickness(1.f);
                        }
                    }
                } else {
                    this->passedSelected = false;
                    for (uint16_t index = 1; index <= p.population; ++index) {
                        peeps[index].shape.setOutlineThickness(0.f);
                    }
                }
            }
        );

        // add everything to gui
        tgui::Panel::Ptr panel = this->rightPanelComponent->getPanel();
        panel->add(this->flowControlComponent->getGroup());
        panel->add(this->bottomButtonsComponent->getGroup());
        this->gui.add(panel);

        this->console = new ConsoleComponent();
        this->gui.add(this->console->getConsole());

        // setup view
        this->viewComponent = new ViewComponent(this->window->getSize());
        this->view = this->viewComponent->getView();
    }

    /**
     * Initialize file dialog for saving a simulation
     */
    void SFMLUserIO::initSaveFileDialog()
    {
        if (this->saveFileDialog == nullptr) 
        {
            this->saveFileDialog = tgui::FileDialog::create("Save file", "Save");
            this->saveFileDialog->setMultiSelect(false);
            this->saveFileDialog->setFileMustExist(false);
            this->saveFileDialog->setPath("Output/Saves");
            this->saveFileDialog->setFilename("simulation.bin");
            //this->saveFileDialog->setFileTypeFilters({ {"All files", {}} }, 1);
            this->saveFileDialog->setPosition("5%", "5%");

            this->saveFileDialog->onFileSelect([this](const tgui::String& filePath){
                Save::save(filePath.toStdString());
                this->childWindowToggled(false);
            });

            this->saveFileDialog->onCancel([this]{
                this->childWindowToggled(false);
            });
        }
    }

    /**
     * Initialize file dialog for loading a simulation
     */
    void SFMLUserIO::initLoadFileDialog()
    {
        if (this->loadFileDialog == nullptr) {
            this->loadFileDialog = tgui::FileDialog::create("Open file", "Open");
            this->loadFileDialog->setMultiSelect(false);
            this->loadFileDialog->setFileMustExist(true);
            this->loadFileDialog->setPath("Output/Saves");
            this->loadFileDialog->setFilename("simulation.bin");
            //this->saveFileDialog->setFileTypeFilters({ {"All files", {}} }, 1);
            this->loadFileDialog->setPosition("5%", "5%");

            this->loadFileDialog->onFileSelect([this](const tgui::String& filePath){
                this->loadFileSelected = true;
                this->loadFilename = filePath.toStdString();
                this->childWindowToggled(false);
            });
            this->loadFileDialog->onCancel([this]{
                std::cerr << "No file selected.\n";
                this->childWindowToggled(false);
            });    
        }
    }

    /**
     * Update view from parameters
     */
    void SFMLUserIO::setFromParams()
    {
        this->rightPanelComponent->setFromParams();
    }

    /**
     * Called when child window is shown or hidden.
     * If it is shown, view sholdn't respond to user input 
     * and simulation should be paused (see pauseExternal and pauseResume description of flowControlComponent).
     * 
     * @param shown true if shown
     */
    void SFMLUserIO::childWindowToggled(bool shown)
    {
        if (shown)
        {
            this->viewComponent->lock();
        }
        else 
        {
            this->viewComponent->unlock();
        }
        this->flowControlComponent->pauseExternal(shown);
        this->isChildWindowShowing = shown;
    }

    SFMLUserIO::~SFMLUserIO()
    {
        delete this->window;
    }

    /**
     * Check if the simulation should be stopped (e.g. because of closed window)
     */
    bool SFMLUserIO::isStopped()
    {
        return !this->window->isOpen();
    }

    /**
     * User input handling.
     * User interaction with view is handled at viewComponent->updateInput
     */
    void SFMLUserIO::updatePollEvents()
    {
        sf::Event e;
        while (this->window->pollEvent(e))
        {
            if (e.Event::type == sf::Event::Closed)
            {
                this->window->close();
            }

            if (e.Event::type == e.Event::KeyPressed && e.Event::key.code == sf::Keyboard::Key::Escape)
            {
                this->window->close();
            }

            if (e.Event::type == e.Event::KeyPressed && e.Event::key.code == sf::Keyboard::Key::Space)
            {
                this->flowControlComponent->pauseResume(!this->paused);
            }

            // update view
            this->viewComponent->updateInput(e, this->window);

            if (e.Event::type == sf::Event::MouseButtonReleased)
            {
                // select indiv on mouse position on RMB
                if (e.mouseButton.button == 1 && !this->passedSelected)
                {
                    int liveDisplayScale = this->getLiveDisplayScale();
                    sf::Vector2f mousePosition = this->window->mapPixelToCoords(sf::Mouse::getPosition(*window));
                    std::cout << mousePosition.x << " " << mousePosition.y << std::endl;

                    int16_t x = floor(mousePosition.x / liveDisplayScale);
                    int16_t y = ceil(p.sizeY - 1 - mousePosition.y/liveDisplayScale);

                    if (x >= 0 && x < p.sizeX && y >= 0 && y < p.sizeY)
                    {
                        uint16_t index = grid.at(x, y);
                        if (this->selectedIndex != 0 && this->selectedIndex != index) {
                            peeps[this->selectedIndex].shape.setOutlineThickness(0.f);
                            this->selectedIndex = 0;
                        }
                        if (index != 0) {
                            peeps[index].shape.setOutlineColor(sf::Color::White);
                            peeps[index].shape.setOutlineThickness(1.f);
                            this->selectedIndex = index;
                        }
                    } else if (this->selectedIndex != 0) {
                        peeps[this->selectedIndex].shape.setOutlineThickness(0.f);
                        this->selectedIndex = 0;
                    }
                }
            }

            // hanlde tgui events
            this->gui.handleEvent(e);
        }
    }

    /**
     * Start of a new generation
     */
    void SFMLUserIO::startNewGeneration(unsigned generation, unsigned stepsPerGeneration)
    {
        this->flowControlComponent->startNewGeneration(generation, stepsPerGeneration);
        this->updatePollEvents();
        if (this->stopAtStart)
        {
            this->flowControlComponent->pauseResume(true);
            this->flowControlComponent->flushStopAtSmthButtons();
        }

        this->bottomButtonsComponent->flushRestartButton();

        // init barriers
        int liveDisplayScale = p.displayScale / 1.5;
        barriesrs.clear();
        auto const &barrierLocs = grid.getBarrierLocations();
        for (Coord loc : barrierLocs) {
            sf::RectangleShape barrier = sf::RectangleShape(sf::Vector2f(1.f*liveDisplayScale, 1.f*liveDisplayScale));
            barrier.setPosition((loc.x) * liveDisplayScale, ((p.sizeY - (loc.y + 1.f))) * liveDisplayScale);
            barrier.setFillColor(sf::Color(136, 136, 136));
            barriesrs.push_back(barrier);
        }

        survivalCriteriaManager.initShapes(liveDisplayScale);

        // clear manual selection
        if (this->selectedIndex != 0) {
            peeps[this->selectedIndex].shape.setOutlineThickness(0.f);
            this->selectedIndex = 0;
        }
        
        // clear survival selection
        this->bottomButtonsComponent->switchPassedSelection(false);        
    }

    /**
     * End of a step - main display loop.
     * Displays all element of the simulation and gui.
     * Also controlls the speed of the simulation by skipping or redrawing frames
     */
    void SFMLUserIO::endOfStep(unsigned simStep)
    {
        // initiate stop before last frame if stopAtEnd is triggered
        if (this->stopAtEnd && simStep == p.stepsPerGeneration - 2) {
            this->flowControlComponent->pauseResume(true);
            this->flowControlComponent->flushStopAtSmthButtons();
        }
        // handle increase speed by skipping frames
        if (this->increaseSpeedCounter < this->speedThreshold)
        {
            this->increaseSpeedCounter++;
            return;
        }
        this->increaseSpeedCounter = 0;

        do 
        {
            this->updatePollEvents();

            this->flowControlComponent->endOfStep(simStep);
            this->window->setView(*this->view);

            this->window->clear();

            // display population
            int liveDisplayScale = this->getLiveDisplayScale();
            for (uint16_t index = 1; index <= p.population; ++index)
            {
                Indiv &indiv = peeps[index];
                if (indiv.alive)
                {
                    indiv.shape.setPosition(
                        static_cast<float>(indiv.loc.x * liveDisplayScale),
                        static_cast<float>(((p.sizeY - indiv.loc.y) - 1) * liveDisplayScale));
                    this->window->draw(indiv.shape);
                }
            }

            // display barriers
            for (sf::RectangleShape &barrier : barriesrs) {
                this->window->draw(barrier);
            }
            
            // display survival criterias
            for (sf::Drawable *shape : survivalCriteriaManager.getShapes()) {
                this->window->draw(*shape);
            }

            this->gui.draw();

            this->window->display();

            //handle slow time by drawing speedThreshold unchanged frames
            this->slowSpeedCounter--;

        } while (this->slowSpeedCounter >= this->speedThreshold);
        this->slowSpeedCounter = 0;
    }

    /**
     * Get live display scale - how big or small the display should be
     * compared with the imageWriter display scale
     */
    int SFMLUserIO::getLiveDisplayScale()
    {
        return p.displayScale / 1.5;
    }

    void SFMLUserIO::endOfGeneration(unsigned generation)
    {
    }

    /**
     * Log message into gui console
     */
    void SFMLUserIO::log(std::string message)
    {
        this->console->log(message);
    }

    void SFMLUserIO::pauseResume(bool paused)
    {
        this->paused = paused;
    }

    bool SFMLUserIO::isPaused()
    {
        return this->paused;
    }

    /**
     * Called when user changed settings via gui.
     * Delegates to paramManager 
     * 
     * @param name parameter name
     * @param val parameter value
     */
    void SFMLUserIO::settingsChanged(std::string name, std::string val)
    {
        paramManager.changeFromUi(name, val);
    }

    void SFMLUserIO::speedChanged(float value)
    {
        this->speedThreshold = value;
    }
}
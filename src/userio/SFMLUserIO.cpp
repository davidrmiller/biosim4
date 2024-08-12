#include "SFMLUserIO.h"

namespace BS
{
    SFMLUserIO::SFMLUserIO()
    {
        this->window = new sf::RenderWindow(sf::VideoMode(this->windowWidth, this->windowHeight), "biosim4", sf::Style::Close | sf::Style::Titlebar);
        this->window->setFramerateLimit(144);
        this->window->setVerticalSyncEnabled(false);
        this->window->setPosition(sf::Vector2i(200, (sf::VideoMode::getDesktopMode().height - this->windowHeight) / 2));

        // setup gui
        this->gui.setWindow(*this->window);
        tgui::Theme::setDefault("./Resources/Black.txt");

        // setup left panel
        this->rightPanelComponent = new RightPanelComponent(
            this->window->getSize(),
            [this](std::string name, std::string val)
            {
                this->settingsChanged(name, val);
            },            
            [this]()
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

        this->flowControlComponent = new FlowControlComponent(
            SPEED_SLOW_MAX,
            SPEED_FAST_MAX,
            0,
            [this](float value)
            {
                this->speedChanged(value);
            },
            [this](bool paused)
            {
                this->pauseResume(paused);
            },
            [this](bool stopAtStart, bool stopAtEnd)
            {
                this->stopAtEnd = stopAtEnd; 
                this->stopAtStart = stopAtStart;
            }
        );
        
        this->infoWindowComponent = new InfoWindowComponent([this]{
            this->childWindowToggled(false);
        });
        this->bottomButtonsComponent = new BottomButtonsComponent(
            [this](void)
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
            [this]()
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
            [this](bool restart)
            {
                this->restartOnEnd = restart;
            },
            [this]()
            {
                this->flowControlComponent->pauseResume(true);
                
                if (this->selectedIndex != 0) {
                    std::string filename = Save::saveNet(this->selectedIndex);                    
                    this->log("Saved into: " + filename);
                }
            },
            [this](std::string name, std::string val)
            {
                this->settingsChanged(name, val);
            },
            [this]()
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
                }
                this->childWindowToggled(true);
                this->gui.add(this->infoWindowComponent->getChildWindow());
            }
        );

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

    void SFMLUserIO::setFromParams()
    {
        this->rightPanelComponent->setFromParams();
    }

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

    bool SFMLUserIO::isStopped()
    {
        return !this->window->isOpen();
    }

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

            this->viewComponent->updateInput(e, this->window);

            if (e.Event::type == sf::Event::MouseButtonReleased)
            {
                if (e.mouseButton.button == 1)
                {
                    // select indiv on mouse position
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

            this->gui.handleEvent(e);
        }
    }

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


        int liveDisplayScale = p.displayScale / 1.5;
        barriesrs.clear();
        auto const &barrierLocs = grid.getBarrierLocations();
        for (Coord loc : barrierLocs) {
            sf::RectangleShape barrier = sf::RectangleShape(sf::Vector2f(1.f*liveDisplayScale, 1.f*liveDisplayScale));
            barrier.setPosition((loc.x) * liveDisplayScale, ((p.sizeY - (loc.y + 1.f))) * liveDisplayScale);
            barrier.setFillColor(sf::Color(136, 136, 136));
            barriesrs.push_back(barrier);
        }

        if (this->selectedIndex != 0) {
            peeps[this->selectedIndex].shape.setOutlineThickness(0.f);
            this->selectedIndex = 0;
        }
    }

    void SFMLUserIO::endOfStep(unsigned simStep)
    {
        // initiate stop before last frame
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

            //display population
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

            for (sf::RectangleShape &barrier : barriesrs) {
                this->window->draw(barrier);
            }

            this->gui.draw();

            this->window->display();

            //handle slow time by drawing speedThreshold unchanged frames
            this->slowSpeedCounter--;

        } while (this->slowSpeedCounter >= this->speedThreshold);
        this->slowSpeedCounter = 0;
    }

    int SFMLUserIO::getLiveDisplayScale()
    {
        return p.displayScale / 1.5;
    }

    void SFMLUserIO::endOfGeneration(unsigned generation)
    {
    }

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

    void SFMLUserIO::settingsChanged(std::string name, std::string val)
    {
        paramManager.changeFromUi(name, val);
    }

    void SFMLUserIO::speedChanged(float value)
    {
        this->speedThreshold = value;
    }
}
#include "SFMLUserIO.h"
#include "../simulator.h"
#include "../indiv.h"
#include "../params.h"

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
        this->rightPanelComponent = new RightPanelComponent(this->window->getSize(), [this](bool paused) {this->pauseResume(paused); });

        ChallengeBoxComponent* challengeBoxComponent = new ChallengeBoxComponent([this](std::string name, std::string val) {
            this->settingsChanged(name, val);
        });
        this->rightPanelComponent->addToPanel(challengeBoxComponent->getChallengeBox());

        this->gui.add(this->rightPanelComponent->getPanel());        

        // setup view
        this->viewComponent = new ViewComponent(this->window->getSize());
        this->view = this->viewComponent->getView();
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

            this->viewComponent->updateInput(e, sf::Mouse::getPosition(*this->window));

            this->gui.handleEvent(e);
        }
    }

    void SFMLUserIO::startNewGeneration(unsigned generation, unsigned stepsPerGeneration)
    {
        this->rightPanelComponent->startNewGeneration(generation, stepsPerGeneration);
    }

    void SFMLUserIO::endOfStep(unsigned simStep)
    {
        this->rightPanelComponent->endOfStep(simStep);
        this->window->setView(*this->view);

        this->window->clear();

        int liveDisplayScale = p.displayScale / 1.5;
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

        this->gui.draw();

        this->window->display();
    }

    void SFMLUserIO::endOfGeneration(unsigned generation)
    {
    }

    void SFMLUserIO::log(std::string message)
    {
        this->rightPanelComponent->log(message);
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
        std::cout << "Got settings: " << name << " value: " << val << std::endl;
    }
}
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

        // create panel
        tgui::Panel::Ptr panel = tgui::Panel::create();
        panel->setSize("20%", this->windowHeight);
        panel->setAutoLayout(tgui::AutoLayout::Right);
        this->gui.add(panel);

        // setup console
        this->console = tgui::TextArea::create();
        this->console->setReadOnly(true);
        this->console->setSize("100%", "50%");
        this->console->setPosition("0%", "50%");
        panel->add(this->console);

        // setup pause button
        tgui::Button::Ptr button = tgui::Button::create("Pause/Resume");
        button->setPosition("20%", 50);
        button->onPress([this](){this->pauseResume();});
        panel->add(button, "Pause/Resume");

        pausePicture = tgui::Picture::create("Resources/Pictures/play.png");
        pausePicture->onClick([this](){this->pauseResume();});
        pausePicture->setPosition("0%", 0);
        panel->add(pausePicture, "Picture");

        // setup edit box
        auto editBox = tgui::EditBox::create();
        editBox->setPosition("20%", 150);
        panel->add(editBox, "MyEditBox");

        // setup view
        this->view = new sf::View(sf::FloatRect(0, 0, this->windowWidth, this->windowHeight));
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

            if (e.Event::type == e.Event::KeyReleased) 
            {
                if (e.Event::key.code == sf::Keyboard::Key::Add)
                {
                    this->view->zoom( 0.75f );
                } 
                else if (e.Event::key.code == sf::Keyboard::Key::Subtract)
                {
                    this->view->zoom( 1.5f );
                }
            }

            // #region View navigation
            if (e.Event::type == sf::Event::MouseButtonPressed) 
            {
                // Mouse button is pressed, get the position and set moving as active
                if (e.mouseButton.button == 0) {
                    this->viewIsMoving = true;
                    this->oldViewPos = sf::Vector2f(sf::Mouse::getPosition(*this->window));
                }
            }

            if (e.Event::type == sf::Event::MouseButtonReleased) 
            {
                if (e.mouseButton.button == 0) {
                    this->viewIsMoving = false;
                }
            }

            // Drag mouse functionality
            if (e.Event::type == sf::Event::MouseMoved && this->viewIsMoving)
            {
                // Determine the new position in world coordinates
                const sf::Vector2f newPos = sf::Vector2f(e.mouseMove.x, e.mouseMove.y);
                // Determine how the cursor has moved
                // Swap these to invert the movement direction
                sf::Vector2f deltaPos = this->oldViewPos - newPos;

                // Applying zoom "reduction" (or "augmentation")
                deltaPos.x *= accumZoom;
                deltaPos.y *= accumZoom;

                // Move our view accordingly and update the window
                this->view->move(deltaPos); // <-- Here I use move

                // Save the new position as the old one
                // We're recalculating this, since we've changed the view
                this->oldViewPos = newPos; // With move, I don't need to recalculate
            }

            // Zoom functionality
            if (e.Event::type == e.Event::MouseWheelMoved && !this->viewIsMoving)
            {
                //ToDo: make it work with any value
                float zoomOutFactor = 0.5f;
                float zoomInFactor = 2.f;

                float zoom = e.mouseWheel.delta > 0 ? zoomOutFactor : zoomInFactor;
                float tmpZoom = this->accumZoom * zoom;
                if (tmpZoom >= 0.125f && tmpZoom <= 2.f) 
                {
                    this->accumZoom *= zoom;
                    this->view->zoom(zoom);

                    float zoomFactor = this->view->getSize().y / this->window->getSize().y;

                    sf::Vector2f newpos(this->window->getSize().x, window->getSize().y);
                    newpos /= zoomInFactor;
                    newpos -= sf::Vector2f(sf::Mouse::getPosition(*this->window).x, sf::Mouse::getPosition(*this->window).y);
                    newpos *= e.mouseWheel.delta > 0 ? zoomFactor : zoomFactor / zoomInFactor;
                    this->view->move(e.mouseWheel.delta > 0 ? -newpos : newpos);
                }
            }

            // #endregion

            // #pragma region View movement
            float const viewSpeed  = 2.f;
            if (e.Event::type == e.Event::KeyPressed && e.Event::key.code == sf::Keyboard::Left)
            {
                this->view->move(sf::Vector2f(-viewSpeed, 0));
            }
            else if (e.Event::type == e.Event::KeyPressed && e.Event::key.code == sf::Keyboard::Right)
            {
                this->view->move(sf::Vector2f(viewSpeed, 0));
            }
            if (e.Event::type == e.Event::KeyPressed && e.Event::key.code == sf::Keyboard::Up)
            {
                this->view->move(sf::Vector2f(0, -viewSpeed));
            }
            else if (e.Event::type == e.Event::KeyPressed && e.Event::key.code == sf::Keyboard::Down)
            {
                this->view->move(sf::Vector2f(0, viewSpeed));
            }
            // #pragma endregion

            this->gui.handleEvent(e);
        }
    }

    void SFMLUserIO::updateInput()
    {
    }

    void SFMLUserIO::endOfStep()
    {
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
        this->logMessages.push_back(tgui::String(message));
        if (this->logMessages.size() > 100)
        {
            this->logMessages.erase(this->logMessages.begin());
        }
        this->console->setText(tgui::String::join( this->logMessages, "\n"));
    }

    void SFMLUserIO::pauseResume()
    {
        this->paused = !this->paused;
        if (this->paused)
            this->pausePicture->getRenderer()->setTexture("Resources/Pictures/pause.png");
        else 
            this->pausePicture->getRenderer()->setTexture("Resources/Pictures/play.png");
    }

    bool SFMLUserIO::isPaused()
    {
        return this->paused;
    }
}
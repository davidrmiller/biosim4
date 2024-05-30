#include "SFMLView.h"
#include "../simulator.h"
#include "../indiv.h"
#include "../params.h"

namespace BS
{
    SFMLView::SFMLView()
    {
        this->window = new sf::RenderWindow(sf::VideoMode(800, 700), "Game3", sf::Style::Close | sf::Style::Titlebar);
        this->window->setFramerateLimit(144);
        this->window->setVerticalSyncEnabled(false);
        this->window->setPosition(sf::Vector2i(500, sf::VideoMode::getDesktopMode().height / 2 - 320));
    }

    SFMLView::~SFMLView()
    {
        delete this->window;
    }

    bool SFMLView::isStopped()
    {
        return !this->window->isOpen();
    }

    void SFMLView::updatePollEvents()
    {
        sf::Event e;
        while (this->window->pollEvent(e))
        {
            if (e.Event::type == sf::Event::Closed)
            {
                this->window->close();
            }
            #pragma GCC diagnostic ignored "-Wint-in-bool-context"
            if (e.Event::KeyPressed && e.Event::key.code == sf::Keyboard::Escape)
            {
                this->window->close();
            }
        }
    }

    void SFMLView::endOfStep()
    {
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

        this->window->display();
    }

    void SFMLView::endOfGeneration(unsigned generation)
    {
    }
}
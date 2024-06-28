#include "ViewComponent.h"

namespace BS
{
    ViewComponent::ViewComponent(sf::Vector2u windowSize)
    {
        this->windowWidth = windowSize.x;
        this->windowHeight = windowSize.y;
        this->view = new sf::View(sf::FloatRect(0, 0, this->windowWidth, this->windowHeight));
        this->view->move(sf::Vector2f(-20.f, -20.f));
    }

    ViewComponent::~ViewComponent()
    {
    }

    void ViewComponent::updateInput(sf::Event e, sf::RenderWindow* window)
    {
        if (this->isLocked)
        {
            return;
        }
        
        // View keyboard zoom
        if (e.Event::type == e.Event::KeyReleased)
        {
            if (e.Event::key.code == sf::Keyboard::Key::Add)
            {
                this->view->zoom(0.8f);
                this->accumZoom *= 0.8f;
            }
            else if (e.Event::key.code == sf::Keyboard::Key::Subtract)
            {
                this->view->zoom(1.25f);
                this->accumZoom *= 1.25f;
            }
        }

        // View mouse navigation trigger
        if (e.Event::type == sf::Event::MouseButtonPressed)
        {
            // Mouse button is pressed, get the position and set moving as active
            if (e.mouseButton.button == 0)
            {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
                if (mousePosition.x > 0 && mousePosition.x < this->windowWidth - (this->windowWidth / 10) * 2)
                {
                
                    this->viewIsMoving = true;
                    this->oldViewPos = sf::Vector2f(mousePosition);
                }
            }
        }

        // View mouse navigation release
        if (e.Event::type == sf::Event::MouseButtonReleased)
        {
            if (e.mouseButton.button == 0)
            {
                this->viewIsMoving = false;
            }
        }

        // View drag mouse functionality
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

        // Zoom view functionality
        if (e.Event::type == e.Event::MouseWheelMoved && !this->viewIsMoving)
        {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
            if (mousePosition.x > 0 && mousePosition.x < this->windowWidth - (this->windowWidth / 10) * 2)
            {
                float zoomOutFactor = 0.8f;
                float zoomInFactor = 1.25f;

                float zoom = e.mouseWheel.delta > 0 ? zoomOutFactor : zoomInFactor;
                float tmpZoom = this->accumZoom * zoom;
                if (tmpZoom >= 0.0625f && tmpZoom <= 2.f)
                {
                    // the idea here is to move center of the view
                    // to oldPos - newPos, divide it by 2 to get the center, 
                    // and make it negative (so we move left if we zoom in and right if we zoom out)
                    // in order to get top left point at the same position as it was before zooming relative to screen

                    // we should also store old and new mouse positions corrected by zoom
                    // get delta between them, and move the view by that amount
                    // in order to get mouse position at the same position as it was before zooming relative to screen
                    
                    sf::Vector2f oldViewVector = this->view->getSize();
                    sf::Vector2f oldRelativeMousePos = sf::Vector2f(mousePosition.x * this->accumZoom, mousePosition.y * this->accumZoom);

                    this->accumZoom *= zoom;
                    this->view->zoom(zoom);
                    
                    sf::Vector2f newViewVector = this->view->getSize();
                    sf::Vector2f newRelativeMousePos = oldRelativeMousePos*zoom;
                    
                    sf::Vector2f moveVectorTopLeft = newViewVector/2.f - oldViewVector/2.f; // simplification of -1*(old/2 - new/2)
                    this->view->move(moveVectorTopLeft);
                    sf::Vector2f moveVectorMousePos = oldRelativeMousePos - newRelativeMousePos;
                    this->view->move(moveVectorMousePos);
                }
            }
        }

        // view keyboard movement
        float const viewSpeed = 2.f;
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
    }

    void ViewComponent::lock()
    {
        this->isLocked = true;
    }

    void ViewComponent::unlock()
    {
        this->isLocked = false;
    }
}
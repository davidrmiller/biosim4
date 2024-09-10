#include <cassert>
#include <utility>

#include "SurvivalCriteria.h"

namespace BS
{
    void SurvivalCriteria::initShapes(int liveDisplayScale)
    {
        //std::cout << "initShapes" << std::endl;
    }

    void SurvivalCriteria::endOfStep(unsigned simStep, const Params &p, Grid &grid, Peeps &peeps) 
    {
        //std::cout << "endOfStep" << std::endl;
    }

    void SurvivalCriteria::deleteShapes()
    {
        for (sf::Drawable *shape : this->shapes)
        {
            delete shape;
        }
        this->shapes.clear();
    }

    void SurvivalCriteria::createCircle(float radius, float x, float y)
    {
        sf::CircleShape* shape = new sf::CircleShape();
        shape->setRadius(radius);
        shape->setPosition(x, y);
        shape->setOutlineThickness(1);
        shape->setOutlineColor(this->defaultColor);
        shape->setFillColor(sf::Color::Transparent);
        this->shapes.push_back(shape);
    }

    void SurvivalCriteria::createBorder(float size, int liveDisplayScale)
    {
        //create border around grid
        sf::Color borderColor = sf::Color::Black;
        sf::RectangleShape* topRect = new sf::RectangleShape();
        topRect->setSize(sf::Vector2f(p.sizeX * liveDisplayScale + size*2, size));
        topRect->setPosition(0 - size, 0 - size);
        topRect->setFillColor(borderColor);
        this->shapes.push_back(topRect);

        sf::RectangleShape* bottomRect = new sf::RectangleShape();
        bottomRect->setSize(sf::Vector2f(p.sizeX * liveDisplayScale + size*2, size));
        bottomRect->setPosition(0 - size, p.sizeY * liveDisplayScale);
        bottomRect->setFillColor(borderColor);
        this->shapes.push_back(bottomRect);

        sf::RectangleShape* leftRect = new sf::RectangleShape();
        leftRect->setSize(sf::Vector2f(size, p.sizeY * liveDisplayScale + size*2));
        leftRect->setPosition(0 - size, 0 - size);
        leftRect->setFillColor(borderColor);
        this->shapes.push_back(leftRect);

        sf::RectangleShape* rightRect = new sf::RectangleShape();
        rightRect->setSize(sf::Vector2f(size, p.sizeY * liveDisplayScale + size*2));
        rightRect->setPosition(p.sizeX * liveDisplayScale, 0 - size);
        rightRect->setFillColor(borderColor);
        this->shapes.push_back(rightRect);
    }

    void SurvivalCriteria::createLine(sf::Vector2f vectorOne, sf::Vector2f vectorTwo)
    {
        sf::VertexArray* lineLeft = new sf::VertexArray(sf::LinesStrip, 2);
        lineLeft->clear();
        sf::Vertex vertexOne = sf::Vertex(vectorOne, this->defaultColor);
        lineLeft->append(vertexOne);
        sf::Vertex vertexTwo = sf::Vertex(vectorTwo, this->defaultColor);
        lineLeft->append(vertexTwo);
        this->shapes.push_back(lineLeft);
    }
}

#include "../include/createGridBarrierVisitor.h"
#include "../include/grid.h"
#include "../include/coord.h"
#include "../include/gridShapeFactory.h"
#include "../include/gridShape.h"

#include <catch2/catch_test_macros.hpp>

namespace BS {

    TEST_CASE("TestCircleVisit","[CreateGridBarrierVisitor]") {
        
        Grid g = Grid();
        uint16_t numLayers = 1;
        uint16_t sizeX = 128;
        uint16_t sizeY = 128;
        g.init(numLayers, sizeX, sizeY);

        CreateGridBarrierVisitor visitor = CreateGridBarrierVisitor(g);

        Coord loc = Coord(64, 64);
        float radius = 5.0;

        std::unique_ptr<GridShape> shape = GridShapeFactory::createGridCircle(g, loc, radius);
        
        CHECK_FALSE(g.isBarrierAt(loc));
        
        shape->accept(visitor);

        CHECK(g.isBarrierAt(loc));

    }

    TEST_CASE("TestCircleVisitRespectsGridBounds","[CreateGridBarrierVisitor]") {
        
        Grid g = Grid();
        uint16_t numLayers = 1;
        uint16_t sizeX = 128;
        uint16_t sizeY = 128;
        g.init(numLayers, sizeX, sizeY);

        CreateGridBarrierVisitor visitor = CreateGridBarrierVisitor(g);
        
        // loc is out of the grid boundaries
        Coord loc = Coord(200, 200);
        float radius = 5.0;

        std::unique_ptr<GridShape> shape = GridShapeFactory::createGridCircle(g, loc, radius);

        CHECK_FALSE(g.isBarrierAt(loc));

        shape->accept(visitor);

        CHECK_FALSE(g.isBarrierAt(loc));

    }

    TEST_CASE("TestRectangleVisit","[CreateGridBarrierVisitor]") {
        
        Grid g = Grid();
        uint16_t numLayers = 1;
        uint16_t sizeX = 128;
        uint16_t sizeY = 128;
        g.init(numLayers, sizeX, sizeY);

        CreateGridBarrierVisitor visitor = CreateGridBarrierVisitor(g);

        Coord min = Coord(54, 54);
        Coord max = Coord(74, 74);

        std::unique_ptr<GridShape> shape = GridShapeFactory::createGridRectangle(g, min, max);

        
        CHECK_FALSE(g.isBarrierAt(min));
        CHECK_FALSE(g.isBarrierAt(max));

        shape->accept(visitor);

        CHECK(g.isBarrierAt({60, 60}));
        CHECK(g.isBarrierAt({70, 70}));

    }

    TEST_CASE("TestRectangleVisitRespectsGridBounds","[CreateGridBarrierVisitor]") {
        
        Grid g = Grid();
        uint16_t numLayers = 1;
        uint16_t sizeX = 128;
        uint16_t sizeY = 128;
        g.init(numLayers, sizeX, sizeY);

        CreateGridBarrierVisitor visitor = CreateGridBarrierVisitor(g);
        
        // loc is out of the grid boundaries
        Coord min = Coord(150, 150);
        Coord max = Coord(200, 200);

        std::unique_ptr<GridShape> shape = GridShapeFactory::createGridRectangle(g, min, max);

        CHECK_FALSE(g.isBarrierAt(min));
        CHECK_FALSE(g.isBarrierAt(max));

        shape->accept(visitor);

        CHECK_FALSE(g.isBarrierAt(min));
        CHECK_FALSE(g.isBarrierAt(max));


    }
}
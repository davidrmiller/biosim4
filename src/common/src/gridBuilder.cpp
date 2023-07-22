#include "../include/gridBuilder.h"
#include "../../random.h"

#include "../include/createGridBarrierVisitor.h"
#include "../include/gridShape.h"
#include "../include/gridShapeFactory.h"

namespace BS {

    GridBuilder::GridBuilder(RandomUintGenerator &r) : randomUint(r) {} 

    /**
     * two use cases, create a new Grid, refresh an existing Grid
     * reset grid & barriers, then add new barriers
    */
    Grid GridBuilder::createBarrier(Grid &g, GridBuilder::barrierType type)
    {
        g.zeroFill();

        CreateGridBarrierVisitor visitor = CreateGridBarrierVisitor(g);

        switch(type) {

            case GridBuilder::barrierType::none:
                return g;

            // Vertical bar in constant location
            case GridBuilder::barrierType::vertical_bar:
            {
                int16_t minX = g.sizeX() / 2;
                int16_t maxX = minX + 1;
                int16_t minY = g.sizeY() / 4;
                int16_t maxY = minY + g.sizeY() / 2;

                std::unique_ptr<GridShape> shape = GridShapeFactory::createGridRectangle(g, {minX, minY}, {maxX, maxY});

                shape->accept(visitor);

            }
               break;

            /** 
             * Vertical bar in random location
             * TODO: must respect size of grid! change hard coded 20 value
             * calculate from grid size x & y
            */
            case GridBuilder::barrierType::random_vertical_bar:
            {
                int16_t minX = randomUint(20, g.sizeX() - 20);
                int16_t maxX = minX + 1;
                int16_t minY = randomUint(20, g.sizeY() / 2 - 20);
                int16_t maxY = minY + g.sizeY() / 2;

                std::unique_ptr<GridShape> shape = GridShapeFactory::createGridRectangle(g, {minX, minY}, {maxX, maxY});

                shape->accept(visitor);

            }
                break;

            // five blocks staggered
            case GridBuilder::barrierType::five_bars_staggered:
            {
                int16_t blockSizeX = 2;
                int16_t blockSizeY = g.sizeX() / 3;

                int16_t x0 = g.sizeX() / 4 - blockSizeX / 2;
                int16_t y0 = g.sizeY() / 4 - blockSizeY / 2;
                int16_t x1 = x0 + blockSizeX;
                int16_t y1 = y0 + blockSizeY;

                std::unique_ptr<GridShape> shape = GridShapeFactory::createGridRectangle(g, {x0, y0}, {x1, y1});
                shape->accept(visitor);

                x0 += g.sizeX() / 2;
                x1 = x0 + blockSizeX;
                shape = GridShapeFactory::createGridRectangle(g, {x0, y0}, {x1, y1});
                shape->accept(visitor);

                y0 += g.sizeY() / 2;
                y1 = y0 + blockSizeY;
                shape = GridShapeFactory::createGridRectangle(g, {x0, y0}, {x1, y1});
                shape->accept(visitor);

                x0 -= g.sizeX() / 2;
                x1 = x0 + blockSizeX;
                shape = GridShapeFactory::createGridRectangle(g, {x0, y0}, {x1, y1});
                shape->accept(visitor);

                x0 = g.sizeX() / 2 - blockSizeX / 2;
                x1 = x0 + blockSizeX;
                y0 = g.sizeY() / 2 - blockSizeY / 2;
                y1 = y0 + blockSizeY;
                shape = GridShapeFactory::createGridRectangle(g, {x0, y0}, {x1, y1});
                shape->accept(visitor);

            }
                break;
            

            // Horizontal bar in constant location
            case GridBuilder::barrierType::horizontal_bar:
            {
                int16_t minX = g.sizeX() / 4;
                int16_t maxX = minX + g.sizeX() / 2;
                int16_t minY = g.sizeY() / 2 + g.sizeY() / 4;
                int16_t maxY = minY + 2;

                std::unique_ptr<GridShape> shape = GridShapeFactory::createGridRectangle(g, {minX, minY}, {maxX, maxY});

                shape->accept(visitor);
            }
                break;

            // Three floating islands -- different locations every generation
            case GridBuilder::barrierType::three_floating_islands: 
                break;

            // Spots, specified number, radius, locations
            case GridBuilder::barrierType::spots:
            {
                // parameterize these values
                unsigned numberOfLocations = 5;
                float radius = 5.0;

                unsigned verticalSliceSize = g.sizeY() / (numberOfLocations + 1);

                for (unsigned n = 1; n <= numberOfLocations; ++n) {
                    Coord loc = { (int16_t)(g.sizeX() / 2),
                                (int16_t)(n * verticalSliceSize) };

                    auto shape = GridShapeFactory::createGridCircle(g, loc, radius);
                    shape->accept(visitor);

                    // TODO: need to find a way to set the barrier center
                    // call Grid method directly?
                    // barrierCenters.push_back(loc);
                    g.addBarrierCenter(loc);
                }

            }

            break;

            // default:
            //     assert(false);

        }
        
        return g;
    }
    
}
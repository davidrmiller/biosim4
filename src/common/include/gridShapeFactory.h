#pragma once

#include <memory>

#include "gridShape.h"
#include "coord.h"
#include "grid.h"

namespace BS
{
    class GridShapeFactory
    {
        private:
            /* data */

        public:
            static std::unique_ptr<GridShape> createGridCircle(Grid &g, Coord l, float r);

            static std::unique_ptr<GridShape> createGridRectangle(Grid &g, Coord min, Coord max);

        // GridShapeFactory(/* args */);
        // GridShapeFactory();
    };
    
} // namespace BS

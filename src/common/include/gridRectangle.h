#pragma once

#include "gridShape.h"
#include "grid.h"
#include "coord.h"

namespace BS {

    class GridRectangle : public GridShape {
        
        public:

            GridRectangle(Grid &g, Coord min, Coord max);

            void accept(GridLocationVisitor &visitor) override;

            // ~GridRectangle();

        private:
            Grid& grid;
            Coord min;
            Coord max;

    };
}
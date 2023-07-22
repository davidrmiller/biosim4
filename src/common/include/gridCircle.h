#pragma once

#include "gridShape.h"
#include "grid.h"
#include "coord.h"

namespace BS {

    class GridCircle : public GridShape {
        
        public:

            GridCircle(Grid &g, Coord l, float r);

            void accept(GridLocationVisitor &visitor) override;

            // ~GridCircle();

        private:
            Grid& grid;
            Coord loc;
            float radius;

    };
}
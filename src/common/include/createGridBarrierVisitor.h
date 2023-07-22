#pragma once

#include "grid.h"
#include "gridLocationVisitor.h"
#include "coord.h"

namespace BS
{   
    /**
     * visitor class that builds a barrier on a Grid
    */
    class CreateGridBarrierVisitor: public GridLocationVisitor {
        
        public:

            CreateGridBarrierVisitor(Grid &g);

            void visit(Coord loc);
            
            // ~CreateGridBarrierVisitor();

        private:
            Grid& grid;
    };

} // namespace BS

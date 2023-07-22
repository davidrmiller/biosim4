#pragma once

#include "grid.h"
#include "gridLocationVisitor.h"
#include "coord.h"

namespace BS
{   
    /**
     * visitor class that sets a signal on a grid layer
    */
    class SetSignalVisitor: public GridLocationVisitor {
        
        public:

            SetSignalVisitor(Grid &g, uint8_t amount);

            void visit(Coord loc);
            
            // ~CreateGridBarrierVisitor();

        private:
            Grid& grid;
            uint8_t amount;
    };

} // namespace BS

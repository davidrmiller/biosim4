#include "../include/createGridBarrierVisitor.h"

namespace BS
{   
    CreateGridBarrierVisitor::CreateGridBarrierVisitor(Grid &g) : grid(g) {}

    /**
     * set a barrier at the parameter location
    */
    void CreateGridBarrierVisitor::visit(Coord loc)
    {
        grid.setBarrier(loc);
    }

}
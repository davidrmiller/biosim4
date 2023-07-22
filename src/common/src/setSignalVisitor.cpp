#include "../include/setSignalVisitor.h"

namespace BS
{   
    SetSignalVisitor::SetSignalVisitor(Grid &g, uint8_t amount_) : grid(g), amount(amount_) {}

    /**
     * sets the signal
    */
    void SetSignalVisitor::visit(Coord loc)
    {
        // grid[layers][layerNum][loc.x][loc.y] =
        //                         std::min<unsigned>(SIGNAL_MAX,
        //                                         grid[layerNum][loc.x][loc.y] + amount);
    }

}
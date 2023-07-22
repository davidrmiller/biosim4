#include "../include/gridRectangle.h"

namespace BS
{
    GridRectangle::GridRectangle(Grid &g, Coord min_, Coord max_) : grid(g), min(min_), max(max_) {}

    // GridRectangle::~GridRectangle()
    // {

    // }

    /**
     * Calls visitor.visit() on all locs within the rectangle
    */
    void GridRectangle::accept(GridLocationVisitor &visitor)
    {
        for (int16_t x = min.x; x <= max.x; ++x) {
            for (int16_t y = min.y; y <= max.y; ++y) {
                
                if (grid.isInBounds(x, y)) {
                    visitor.visit(Coord {x, y} );
                }
            }
        }
    }

} // namespace BS

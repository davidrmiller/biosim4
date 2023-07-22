#include "../include/gridCircle.h"

namespace BS
{
    GridCircle::GridCircle(Grid &g, Coord l, float r) : grid(g), loc(l), radius(r) {}

    // GridCircle::~GridCircle() {}

    void GridCircle::accept(GridLocationVisitor &visitor)
    {
        for (int dx = -std::min<int>(radius, loc.x); dx <= std::min<int>(radius, (grid.sizeX() - loc.x) - 1); ++dx) {
            
            int16_t x = loc.x + dx;
            
            int extentY = (int)sqrt(radius * radius - dx * dx);

            for (int dy = -std::min<int>(extentY, loc.y); dy <= std::min<int>(extentY, (grid.sizeY() - loc.y) - 1); ++dy) {
                int16_t y = loc.y + dy;

                if (grid.isInBounds(x, y)) {
                    visitor.visit(Coord {x, y} );
                }
            }
        }
    } 

} // namespace BS


#include "../include/gridShapeFactory.h"
#include "../include/gridCircle.h"
#include "../include/gridRectangle.h"
namespace BS
{
    std::unique_ptr<GridShape> GridShapeFactory::createGridCircle(Grid &g, Coord l, float r)
    {
        return std::make_unique<GridCircle>(g, l, r);
    }

    std::unique_ptr<GridShape> GridShapeFactory::createGridRectangle(Grid &g, Coord min, Coord max)
    {
        return std::make_unique<GridRectangle>(g, min, max);
    }

} // namespace BS

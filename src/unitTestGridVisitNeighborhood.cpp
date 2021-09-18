// unitTestGridVisitNeighborhood

#include <iostream>
#include "simulator.h"

namespace BS {

void unitTestGridVisitNeighborhood()
{
    // prints each coord:
    auto printLoc = [&](Coord loc){ std::cout << loc.x << ", " << loc.y << std::endl; };

    std::cout << "Test loc 10,10 radius 1" << std::endl;
    visitNeighborhood(Coord { 10, 10 }, 1.0, printLoc);

    std::cout << "\nTest loc 0,0 radius 1" << std::endl;
    visitNeighborhood(Coord { 0, 0 }, 1.0, printLoc);

    std::cout << "\nTest loc 10,10 radius 1.4" << std::endl;
    visitNeighborhood(Coord { 10, 10 }, 1.4, printLoc);

    std::cout << "\nTest loc 10,10 radius 1.5" << std::endl;
    visitNeighborhood(Coord { 10, 10 }, 1.5, printLoc);

    std::cout << "\nTest loc 1,1 radius 1.4" << std::endl;
    visitNeighborhood(Coord { 1, 1 }, 1.4, printLoc);

    std::cout << "\nTest loc 10,10 radius 2.0" << std::endl;
    visitNeighborhood(Coord { 10, 10 }, 2.0, printLoc);

    std::cout << "\nTest loc p.sizeX-1, p.sizeY-1 radius 2.0" << std::endl;
    visitNeighborhood(Coord { (int16_t)(p.sizeX-1), (int16_t)(p.sizeY-1) }, 2.0, printLoc);
}

} // end namespace BS

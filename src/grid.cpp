// grid.cpp

#include <functional>
#include <cassert>
#include "simulator.h"

namespace BS {

// Allocates space for the 2D grid
void Grid::init(uint16_t sizeX, uint16_t sizeY)
{
    auto col = Column(sizeY);
    data = std::vector<Column>(sizeX, col);
}

void Grid::zeroFill() 
{ 
    for (Column &column : data) 
        column.zeroFill(); 
    }

uint16_t Grid::sizeX() const 
{ 
    return data.size(); 
}

uint16_t Grid::sizeY() const 
{ 
    return data[0].size(); 
}

bool Grid::isInBounds(Coord loc) const 
{ 
    return loc.x >= 0 && loc.x < sizeX() && loc.y >= 0 && loc.y < sizeY(); 
}

// Returns the number of locations to the next barrier in the
// specified direction, not including loc. Ignores agents in the way.
// If the distance to the border is less than the longProbeDist distance
// and no barriers are found, returns longProbeDist.
// Returns 0..longProbeDist.
unsigned Grid::longProbeBarrierFwd(Coord loc, Dir dir, unsigned longProbeDist)
{
    assert(longProbeDist > 0);
    unsigned count = 0;
    loc = loc + dir;
    unsigned numLocsToTest = longProbeDist;
    while (numLocsToTest > 0 && isInBounds(loc) && !isBarrierAt(loc)) {
        ++count;
        loc = loc + dir;
        --numLocsToTest;
    }
    if (numLocsToTest > 0 && !isInBounds(loc)) {
        return longProbeDist;
    } else {
        return count;
    }
}

// Converts the number of locations (not including loc) to the next barrier location
// along opposite directions of the specified axis to the sensor range. If no barriers
// are found, the result is sensor mid-range. Ignores agents in the path.
float Grid::getShortProbeBarrierDistance(Coord loc0, Dir dir, unsigned probeDistance)
{
    unsigned countFwd = 0;
    unsigned countRev = 0;
    Coord loc = loc0 + dir;
    unsigned numLocsToTest = probeDistance;
    // Scan positive direction
    while (numLocsToTest > 0 && isInBounds(loc) && !isBarrierAt(loc)) {
        ++countFwd;
        loc = loc + dir;
        --numLocsToTest;
    }
    if (numLocsToTest > 0 && !isInBounds(loc)) {
        countFwd = probeDistance;
    }
    // Scan negative direction
    numLocsToTest = probeDistance;
    loc = loc0 - dir;
    while (numLocsToTest > 0 && isInBounds(loc) && !isBarrierAt(loc)) {
        ++countRev;
        loc = loc - dir;
        --numLocsToTest;
    }
    if (numLocsToTest > 0 && !isInBounds(loc)) {
        countRev = probeDistance;
    }

    float sensorVal = ((countFwd - countRev) + probeDistance); // convert to 0..2*probeDistance
    sensorVal = (sensorVal / 2.0) / probeDistance; // convert to 0.0..1.0
    return sensorVal;
}

// Returns the number of locations to the next agent in the specified
// direction, not including loc. If the probe encounters a boundary or a
// barrier before reaching the longProbeDist distance, returns longProbeDist.
// Returns 0..longProbeDist.
unsigned Grid::longProbePopulationFwd(Coord loc, Dir dir, unsigned longProbeDist)
{
    assert(longProbeDist > 0);
    unsigned count = 0;
    loc = loc + dir;
    unsigned numLocsToTest = longProbeDist;
    while (numLocsToTest > 0 && isInBounds(loc) && isEmptyAt(loc)) {
        ++count;
        loc = loc + dir;
        --numLocsToTest;
    }
    if (numLocsToTest > 0 && (!isInBounds(loc) || isBarrierAt(loc))) {
        return longProbeDist;
    } else {
        return count;
    }
}

/**
 * Finds a random unoccupied location in the grid
 * TODO: rewrite to not depend on globals, which is simple
 * remove the reference to the global grid, use this instance!!
 * remove the use of the global parameter p - use the size of this instance!!
*/
// Coord Grid::findEmptyLocation() const {
//     Coord loc;

//     while (true) {
//         loc.x = randomUint(0, sizeX() - 1);
//         loc.y = randomUint(0, sizeY() - 1);
//         if (isEmptyAt(loc)) {
//             break;
//         }
//     }
//     return loc;
// }


} // end namespace BS

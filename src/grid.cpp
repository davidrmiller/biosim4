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

bool Grid::isInBounds(int16_t x, uint16_t y) const
{
    return x >= 0 && x < sizeX() && y >= 0 && y < sizeY(); 
}

bool Grid::isInBounds(Coord loc) const 
{ 
    return loc.x >= 0 && loc.x < sizeX() && loc.y >= 0 && loc.y < sizeY(); 
}

bool Grid::isEmptyAt(Coord loc) const
{
    return at(loc) == EMPTY; 
}

bool Grid::isBarrierAt(Coord loc) const
{ 
    return at(loc) == BARRIER; 
}

bool Grid::isOccupiedAt(Coord loc) const
{
    return at(loc) != EMPTY && at(loc) != BARRIER;
}

bool Grid::isBorder(Coord loc) const
{
    return loc.x == 0 || loc.x == sizeX() - 1 || loc.y == 0 || loc.y == sizeY() - 1;
}

uint16_t Grid::at(Coord loc) const
{ 
    if (isInBounds(loc)) {
        return data[loc.x][loc.y];
    } else {
        return 0;
    }
}

uint16_t Grid::at(uint16_t x, uint16_t y) const
{ 
    if (isInBounds(x,y) ) {
        return data[x][y];
    } else {
        // s this the correct response?
        return 0;
    }
}

void Grid::set(Coord loc, uint16_t val)
{ 
    if (isInBounds(loc)){
        data[loc.x][loc.y] = val;
    }
}

void Grid::set(uint16_t x, uint16_t y, uint16_t val)
{
    if (isInBounds(x,y)){
        data[x][y] = val;
    }
}

const std::vector<Coord> &Grid::getBarrierLocations() const
{ 
    return barrierLocations;
}

const std::vector<Coord> &Grid::getBarrierCenters() const
{
    return barrierCenters;
}

// Direct access:
Column & Grid::operator[](uint16_t columnXNum)
{ 
    return data[columnXNum];
}

const Column & Grid::operator[](uint16_t columnXNum) const
{ 
    return data[columnXNum];
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

    /**
     * TODO: does this belong in Grid? and if not where?
    */
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


} // end namespace BS

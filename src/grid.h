#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

// The grid is the 2D arena where the agents live.

#include <cstdint>
#include <vector>
#include <functional>
#include "./common/include/coord.h"
#include "./common/include/column.h"

namespace BS {

// Grid is a somewhat dumb 2D container of unsigned 16-bit values.
// Grid understands that the elements are either EMPTY, BARRIER, or
// otherwise an index value into the peeps container.
// The elements are allocated and cleared to EMPTY in the ctor.
// Prefer .at() and .set() for random element access. Or use Grid[x][y]
// for direct access where the y index is the inner loop.
// Element values are not otherwise interpreted by class Grid.

const uint16_t EMPTY = 0; // Index value 0 is reserved
const uint16_t BARRIER = 0xffff;

class Grid {
public:

    void init(uint16_t sizeX, uint16_t sizeY);
    void zeroFill();
    uint16_t sizeX() const;
    uint16_t sizeY() const;

    bool isInBounds(int16_t x, uint16_t y) const;
    bool isInBounds(Coord loc) const;
    bool isEmptyAt(Coord loc) const;
    bool isBarrierAt(Coord loc) const;

    // Occupied means an agent is living there.
    bool isOccupiedAt(Coord loc) const;
    bool isBorder(Coord loc) const;
    uint16_t at(Coord loc) const;
    uint16_t at(uint16_t x, uint16_t y) const;

    void set(Coord loc, uint16_t val);
    void set(uint16_t x, uint16_t y, uint16_t val);
    
    // set this location as a barrier
    void setBarrier(int16_t x, int16_t y);
    void setBarrier(Coord loc);

    void createBarrier(unsigned barrierType);
    const std::vector<Coord> &getBarrierLocations() const;
    const std::vector<Coord> &getBarrierCenters() const;

    // Direct access:
    Column & operator[](uint16_t columnXNum);
    const Column & operator[](uint16_t columnXNum) const;

    unsigned longProbeBarrierFwd(Coord loc, Dir dir, unsigned longProbeDist);
    float getShortProbeBarrierDistance(Coord loc0, Dir dir, unsigned probeDistance);
    unsigned longProbePopulationFwd(Coord loc, Dir dir, unsigned longProbeDist);

private:
    std::vector<Column> data;
    std::vector<Coord> barrierLocations;
    std::vector<Coord> barrierCenters;
};

extern void unitTestGridVisitNeighborhood();

} // end namespace BS

#endif // GRID_H_INCLUDED

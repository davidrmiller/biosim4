#ifndef PEEPS_H_INCLUDED
#define PEEPS_H_INCLUDED

#include <cstdint>
#include <vector>
#include "basicTypes.h"
#include "grid.h"
#include "params.h"
#include "indiv.h"

namespace BS {

struct Indiv;
extern class Grid grid;

// This class keeps track of alive and dead Indiv's and where they
// are in the Grid.
// Peeps allows spawning a live Indiv at a random or specific location
// in the grid, moving Indiv's from one grid location to another, and
// killing any Indiv.
// All the Indiv instances, living and dead, are stored in the private
// .individuals member. The .cull() function will remove dead members and
// replace their slots in the .individuals container with living members
// from the end of the container for compacting the container.
// Each Indiv has an identifying index in the range 1..0xfffe that is
// stored in the Grid at the location where the Indiv resides, such that
// a Grid element value n refers to .individuals[n]. Index value 0 is
// reserved, i.e., .individuals[0] is not a valid individual.
// This class does not manage properties inside Indiv except for the
// Indiv's location in the grid and its aliveness.
class Peeps {
public:
    Peeps(); // makes zero individuals
    void init(unsigned population);
    void queueForDeath(const Indiv &);
    void drainDeathQueue();
    void queueForMove(const Indiv &, Coord newLoc);
    void drainMoveQueue();
    unsigned deathQueueSize() const { return deathQueue.size(); }
    // getIndiv() does no error checking -- check first that loc is occupied
    Indiv & getIndiv(Coord loc) { return individuals[grid.at(loc)]; }
    const Indiv & getIndiv(Coord loc) const { return individuals[grid.at(loc)]; }
    // Direct access:
    Indiv & operator[](uint16_t index) { return individuals[index]; }
    Indiv const & operator[](uint16_t index) const { return individuals[index]; }
private:
    std::vector<Indiv> individuals; // Index value 0 is reserved
    std::vector<uint16_t> deathQueue;
    std::vector<std::pair<uint16_t, Coord>> moveQueue;
};

} // end namespace BS

#endif // PEEPS_H_INCLUDED

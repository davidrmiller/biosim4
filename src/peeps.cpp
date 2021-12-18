// peeps.cpp
// Manages a container of individual agents of type Indiv and their
// locations in the grid container

#include <iostream>
#include <cassert>
#include <numeric>
#include <utility>
#include "simulator.h"

namespace BS {


Peeps::Peeps()
{
}


void Peeps::init(unsigned population)
{
    // Index 0 is reserved, so add one:
    individuals.resize(population + 1);
}


// Safe to call during multithread mode.
// Indiv will remain alive and in-world until end of sim step when
// drainDeathQueue() is called. It's ok if the same agent gets
// queued for death multiple times. It does not make sense to
// call this function for agents already dead.
void Peeps::queueForDeath(const Indiv &indiv)
{
    assert(indiv.alive);

    #pragma omp critical
    {
        deathQueue.push_back(indiv.index);
    }
}


// Called in single-thread mode at end of sim step. This executes all the
// queued deaths, removing the dead agents from the grid.
void Peeps::drainDeathQueue()
{
    for (uint16_t index : deathQueue) {
        auto & indiv = peeps[index];
        grid.set(indiv.loc, 0);
        indiv.alive = false;
    }
    deathQueue.clear();
}


// Safe to call during multithread mode. Indiv won't move until end
// of sim step when drainMoveQueue() is called. Should only be called
// for living agents. It's ok if multiple agents are queued to move
// to the same location; only the first one will actually get moved.
void Peeps::queueForMove(const Indiv &indiv, Coord newLoc)
{
    assert(indiv.alive);

    #pragma omp critical
    {
        auto record = std::make_pair<uint16_t, Coord>(uint16_t(indiv.index), Coord(newLoc));
        moveQueue.push_back(record);
    }
}


// Called in single-thread mode at end of sim step. This executes all the
// queued movements. Each movement is typically one 8-neighbor cell distance
// but this function can move an individual any arbitrary distance. It is
// possible that an agent queued for movement was recently killed when the
// death queue was drained, so we'll ignore already-dead agents.
void Peeps::drainMoveQueue()
{
    for (auto& moveRecord : moveQueue) {
        auto & indiv = peeps[moveRecord.first];
        if (indiv.alive) {
            Coord newLoc = moveRecord.second;
            Dir moveDir = (newLoc - indiv.loc).asDir();
            if (grid.isEmptyAt(newLoc)) {
                grid.set(indiv.loc, 0);
                grid.set(newLoc, indiv.index);
                indiv.loc = newLoc;
                indiv.lastMoveDir = moveDir;
            }
        }
    }
    moveQueue.clear();
}

} // end namespace BS

// indiv.cpp

#include <iostream>
#include <cassert>
#include "simulator.h"

namespace BS {

// This is called when any individual is spawned.
// The responsiveness parameter will be initialized here to maximum value
// of 1.0, then depending on which action activation function is used,
// the default undriven value may be changed to 1.0 or action midrange.
void Indiv::initialize(uint16_t index_, Coord loc_, Genome &&genome_)
{
    index = index_;
    loc = loc_;
    //birthLoc = loc_;
    grid.set(loc_, index_);
    age = 0;
    oscPeriod = 34; // ToDo !!! define a constant
    alive = true;
    lastMoveDir = Dir::random8();
    responsiveness = 0.5; // range 0.0..1.0
    longProbeDist = p.longProbeDistance;
    challengeBits = (unsigned)false; // will be set true when some task gets accomplished
    genome = std::move(genome_);
    createWiringFromGenome();
}

} // end namespace BS

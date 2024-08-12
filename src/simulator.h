#ifndef SIMULATOR_H_INCLUDED
#define SIMULATOR_H_INCLUDED

// Main header for the simulator. Also see simulator.cpp.

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <omp.h>

#include "basicTypes.h"   // types Dir, Coord, Polar and their values
#include "params.h"       // params from the config file plus some extra stuff
#include "grid.h"         // the 2D world where the peeps live
#include "peeps.h"        // the 2D world where the peeps live
#include "./ai/indiv.h"        // data structure for an individual
#include "./ai/signals.h"      // a 2D array of pheromones that overlay the world grid
#include "./utils/random.h"
#include "./utils/ProfilingInstrumentor.h"
#include "./userio/UserIO.h"
#include "./utils/Save.h"
#include "./survivalCriteria/survivalCriteriaManager.h"

namespace BS {

extern ParamManager paramManager; // manages simulator params from the config file plus more
extern const Params &p; // read-only simulator config params
extern Grid grid;  // 2D arena where the individuals live
extern Signals signals;  // pheromone layers
extern Peeps peeps;   // container of all the individuals
extern SurvivalCriteriaManager survivalCriteriaManager;
extern UserIO* userIO;
extern void simulator(int argc, char **argv);
void simGeneration(unsigned int &murderCount, unsigned int generation);
void simEndGeneration(unsigned int &murderCount, unsigned int &generation);

// Feeds in-bounds Coords to a function: given a center location and a radius, this
// function will call f(Coord) once for each location inside the specified area.
extern void visitNeighborhood(Coord loc, float radius, std::function<void(Coord)> f);

} // end namespace BS

#endif // SIMULATOR_H_INCLUDED

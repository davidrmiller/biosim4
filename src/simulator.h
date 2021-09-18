#ifndef SIMULATOR_H_INCLUDED
#define SIMULATOR_H_INCLUDED

// Main header for the simulator. Also see simulator.cpp.

#include "basicTypes.h"   // types Dir, Coord, Polar and their values
#include "params.h"       // params from the config file plus some extra stuff
#include "indiv.h"        // data structure for an individual
#include "grid.h"         // the 2D world where the peeps live
#include "signals.h"      // a 2D array of pheromones that overlay the world grid
#include "peeps.h"        // the 2D world where the peeps live
#include "random.h"

namespace BS {

// Some of the survival challenges to try. Some are interesting, some
// not so much. Fine-tune the challenges by tweaking the corresponding code
// in survival-criteria.cpp.
constexpr unsigned CHALLENGE_CIRCLE = 0;
constexpr unsigned CHALLENGE_RIGHT_HALF = 1;
constexpr unsigned CHALLENGE_RIGHT_QUARTER = 2;
constexpr unsigned CHALLENGE_STRING = 3;
constexpr unsigned CHALLENGE_CENTER_WEIGHTED = 4;
constexpr unsigned CHALLENGE_CENTER_UNWEIGHTED = 40;
constexpr unsigned CHALLENGE_CORNER = 5;
constexpr unsigned CHALLENGE_CORNER_WEIGHTED = 6;
constexpr unsigned CHALLENGE_MIGRATE_DISTANCE = 7;
constexpr unsigned CHALLENGE_CENTER_SPARSE = 8;
constexpr unsigned CHALLENGE_LEFT_EIGHTH = 9;
constexpr unsigned CHALLENGE_RADIOACTIVE_WALLS = 10;
constexpr unsigned CHALLENGE_AGAINST_ANY_WALL = 11;
constexpr unsigned CHALLENGE_TOUCH_ANY_WALL = 12;
constexpr unsigned CHALLENGE_EAST_WEST_EIGHTHS = 13;
constexpr unsigned CHALLENGE_NEAR_BARRIER = 14;
constexpr unsigned CHALLENGE_PAIRS = 15;
constexpr unsigned CHALLENGE_LOCATION_SEQUENCE = 16;
constexpr unsigned CHALLENGE_ALTRUISM = 17;
constexpr unsigned CHALLENGE_ALTRUISM_SACRIFICE = 18;

extern ParamManager paramManager; // manages simulator params from the config file plus more
extern const Params &p; // read-only simulator config params
extern Grid grid;  // 2D arena where the individuals live
extern Signals signals;  // pheromone layers
extern Peeps peeps;   // container of all the individuals
extern void simulator(int argc, char **argv);

// Feeds in-bounds Coords to a function: given a center location and a radius, this
// function will call f(Coord) once for each location inside the specified area.
extern void visitNeighborhood(Coord loc, float radius, std::function<void(Coord)> f);

} // end namespace BS

#endif // SIMULATOR_H_INCLUDED

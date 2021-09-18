#ifndef SENSORS_ACTIONS_H_INCLUDED
#define SENSORS_ACTIONS_H_INCLUDED

// This file defines which sensor input neurons and which action output neurons
// are compiled into the simulator. This file can be modified to create a simulator
// executable that supports only a subset of all possible sensor or action neurons.

#include <string>

namespace BS {

// Neuron Sources (Sensors) and Sinks (Actions)

// These sensor, neuron, and action value ranges are here for documentation
// purposes. Most functions now assume these ranges. We no longer support changes
// to these ranges.
constexpr float SENSOR_MIN = 0.0;
constexpr float SENSOR_MAX = 1.0;
constexpr float SENSOR_RANGE = SENSOR_MAX - SENSOR_MIN;

constexpr float NEURON_MIN = -1.0;
constexpr float NEURON_MAX = 1.0;
constexpr float NEURON_RANGE = NEURON_MAX - NEURON_MIN;

constexpr float ACTION_MIN = 0.0;
constexpr float ACTION_MAX = 1.0;
constexpr float ACTION_RANGE = ACTION_MAX - ACTION_MIN;


// Place the sensor neuron you want enabled prior to NUM_SENSES. Any
// that are after NUM_SENSES will be disabled in the simulator.
// If new items are added to this enum, also update the name functions
// in analysis.cpp.
// I means data about the individual, mainly stored in Indiv
// W means data about the environment, mainly stored in Peeps or Grid
enum Sensor {
    LOC_X,             // I distance from left edge
    LOC_Y,             // I distance from bottom
    BOUNDARY_DIST_X,   // I X distance to nearest edge of world
    BOUNDARY_DIST,     // I distance to nearest edge of world
    BOUNDARY_DIST_Y,   // I Y distance to nearest edge of world
    GENETIC_SIM_FWD,   // I genetic similarity forward
    LAST_MOVE_DIR_X,   // I +- amount of X movement in last movement
    LAST_MOVE_DIR_Y,   // I +- amount of Y movement in last movement
    LONGPROBE_POP_FWD, // W long look for population forward
    LONGPROBE_BAR_FWD, // W long look for barriers forward
    POPULATION,        // W population density in neighborhood
    POPULATION_FWD,    // W population density in the forward-reverse axis
    POPULATION_LR,     // W population density in the left-right axis
    OSC1,              // I oscillator +-value
    AGE,               // I
    BARRIER_FWD,       // W neighborhood barrier distance forward-reverse axis
    BARRIER_LR,        // W neighborhood barrier distance left-right axis
    RANDOM,            //   random sensor value, uniform distribution
    SIGNAL0,           // W strength of signal0 in neighborhood
    SIGNAL0_FWD,       // W strength of signal0 in the forward-reverse axis
    SIGNAL0_LR,        // W strength of signal0 in the left-right axis
    NUM_SENSES,        // <<------------------ END OF ACTIVE SENSES MARKER
};


// Place the action neuron you want enabled prior to NUM_ACTIONS. Any
// that are after NUM_ACTIONS will be disabled in the simulator.
// If new items are added to this enum, also update the name functions
// in analysis.cpp.
// I means the action affects the individual internally (Indiv)
// W means the action also affects the environment (Peeps or Grid)
enum Action {
    MOVE_X,                   // W +- X component of movement
    MOVE_Y,                   // W +- Y component of movement
    MOVE_FORWARD,             // W continue last direction
    MOVE_RL,                  // W +- component of movement
    MOVE_RANDOM,              // W
    SET_OSCILLATOR_PERIOD,    // I
    SET_LONGPROBE_DIST,       // I
    SET_RESPONSIVENESS,       // I
    EMIT_SIGNAL0,             // W
    MOVE_EAST,                // W
    MOVE_WEST,                // W
    MOVE_NORTH,               // W
    MOVE_SOUTH,               // W
    MOVE_LEFT,                // W
    MOVE_RIGHT,               // W
    MOVE_REVERSE,             // W
    NUM_ACTIONS,       // <<----------------- END OF ACTIVE ACTIONS MARKER
    KILL_FORWARD,             // W
};

extern std::string sensorName(Sensor sensor);
extern std::string actionName(Action action);
extern void printSensorsActions(); // list the names to stdout

} // end namespace BS

#endif // SENSORS_ACTIONS_H_INCLUDED

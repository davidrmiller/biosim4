#ifndef INDIV_H_INCLUDED
#define INDIV_H_INCLUDED

// Indiv is the structure that represents one individual agent.

#include <algorithm>
#include <cstdint>
#include <array>
#include "basicTypes.h"
#include "genome-neurons.h"

namespace BS {

// Also see class Peeps.

struct Indiv {
    bool alive;
    uint16_t index; // index into peeps[] container
    Coord loc;   // refers to a location in grid[][]
    Coord birthLoc;
    unsigned age;
    Genome genome;
    NeuralNet nnet;   // derived from .genome
    float responsiveness;  // 0.0..1.0 (0 is like asleep)
    unsigned oscPeriod; // 2..4*p.stepsPerGeneration (TBD, see executeActions())
    unsigned longProbeDist; // distance for long forward probe for obstructions
    Dir lastMoveDir;  // direction of last movement
    unsigned challengeBits; // modified when the indiv accomplishes some task
    std::array<float, Action::NUM_ACTIONS> feedForward(unsigned simStep); // reads sensors, returns actions
    float getSensor(Sensor, unsigned simStep) const;
    void initialize(uint16_t index, Coord loc, Genome &&genome);
    void createWiringFromGenome(); // creates .nnet member from .genome member
    void printNeuralNet() const;
    void printIGraphEdgeList() const;
    void printGenome() const;
};

} // end namespace BS

#endif // INDIV_H_INCLUDED

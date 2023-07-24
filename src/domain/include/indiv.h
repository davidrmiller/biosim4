#ifndef INDIV_H_INCLUDED
#define INDIV_H_INCLUDED

// Indiv is the structure that represents one individual agent.
#include <memory>
#include <algorithm>
#include <cstdint>
#include <array>

#include "../../common/include/coord.h"
#include "../../common/include/dir.h"
#include "./genome.h"
#include "./connectionList.h"

#include "../../sensors-actions.h"
#include "./neuralNet.h"

namespace BS {

// Also see class Peeps.

// constexpr float initialNeuronOutput() { return 0.5; }

class Indiv {

    public:

        bool alive;
        uint16_t index; // index into peeps[] container
        Coord loc;   // refers to a location in grid[][]
        
        unsigned age;
        
        // keep a reference to the genome? use a smart ptr to avoid copying
        std::shared_ptr<Genome> genome;

        // peeps[index].nnet->connections.empty()
        // add a method to test if this indiv's nnet connections a empty
        std::shared_ptr<NeuralNet> nnet;   // derived from .genome

        float responsiveness;  // 0.0..1.0 (0 is like asleep)
        unsigned oscPeriod; // 2..4*p.stepsPerGeneration (TBD, see executeActions())
        unsigned longProbeDist; // distance for long forward probe for obstructions
        Dir lastMoveDir;  // direction of last movement
        unsigned challengeBits; // modified when the indiv accomplishes some task
        

        void initialize(uint16_t index, Coord loc, std::shared_ptr<Genome> genome, std::shared_ptr<NeuralNet> nnet, Dir lastMove, unsigned dist);

        std::array<float, Action::NUM_ACTIONS> feedForward(unsigned simStep); // reads sensors, returns actions
        
        float distanceMoved()const;

        // make getSensor private
        float getSensor(Sensor sensorNum, unsigned simStep) const;

        // void createWiringFromGenome(); // creates .nnet member from .genome member
        
        // void printNeuralNet() const;
        
        // move to a presentation class
        void printIGraphEdgeList() const;
        // move to a presentation class
        void printGenome() const;
    
    private:

        Coord birthLoc;
};

} // end namespace BS

#endif // INDIV_H_INCLUDED

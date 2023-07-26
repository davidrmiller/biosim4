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
#include "./neuralNet.h"

#include "../../sensors-actions.h"

namespace BS {

class Indiv {

    public:

        bool alive;

        // index into peeps[] container
        uint16_t index;
        
        // refers to a location in a grid
        Coord loc;
        
        unsigned age;
        
        // this Indiv's genome
        std::shared_ptr<Genome> genome;

        // derived from .genome
        std::shared_ptr<NeuralNet> nnet;   

        // 0.0..1.0 (0 is like asleep)
        float responsiveness;

        // 2..4*p.stepsPerGeneration (TBD, see executeActions())
        unsigned oscPeriod;

        // distance for long forward probe for obstructions
        unsigned longProbeDist;

        // direction of last movement
        Dir lastMoveDir;

        // modified when the indiv accomplishes some task
        unsigned challengeBits;
        
        void initialize(uint16_t index, Coord loc, std::shared_ptr<Genome> genome, std::shared_ptr<NeuralNet> nnet, Dir lastMove, unsigned dist);
        
        // reads sensors, returns actions
        std::array<float, Action::NUM_ACTIONS> feedForward(unsigned simStep);
        
        float distanceMoved() const;

    private:

        Coord birthLoc;

        float getSensor(Sensor sensorNum, unsigned simStep) const;

};

} // end namespace BS

#endif // INDIV_H_INCLUDED

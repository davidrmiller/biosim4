// unitTestConnectNeuralNetWiringFromGenome.cpp

#include <iostream>
#include "simulator.h"

namespace BS {

void unitTestConnectNeuralNetWiringFromGenome()
{
    Indiv indiv;
    Genome genome1 {
//        { SENSOR, 0, NEURON, 0, 0.0 },
//        { SENSOR, 1, NEURON, 2, 2.2 },
//        { SENSOR, 13, NEURON, 9, 3.3 },
//        { NEURON, 4, NEURON, 5, 4.4 },
//        { NEURON, 4, NEURON, 4, 5.5 },
//        { NEURON, 5, NEURON, 9, 6.6 },
//        { NEURON, 0, NEURON, 0, 7.7 },
//        { NEURON, 5, NEURON, 9, 8.8 },
//        { SENSOR, 0, ACTION, 1, 9.9 },
//        { SENSOR, 2, ACTION, 12, 10.1 },
//        { NEURON, 0, ACTION, 1, 11.0 },
//        { NEURON, 4, ACTION, 2, 12.0 }
    };

    indiv.genome = { genome1 };

    indiv.createWiringFromGenome();

    for (auto & conn : indiv.nnet.connections) {
        std::cout << (conn.sourceType == SENSOR ? "SENSOR" : "NEURON") << " "
                  << conn.sourceNum << " -> "
                  << (conn.sinkType == ACTION ? "ACTION" : "NEURON") << " "
                  << conn.sinkNum << " at " << conn.weight << std::endl;
    }

//    for (unsigned n = 0; n < indiv.nnet.neurons.size(); ++n) {
//        std::cout << n << " " << indiv.nnet.neurons[n].numInputs << " inputs" << std::endl;
//    }
}

} // end namespace BS

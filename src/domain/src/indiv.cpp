#include <iostream>
#include <cassert>

#include "../include/indiv.h"
#include "../../simulator.h"
// #include "../../dirFactory.h"

namespace BS {

    // This is called when any individual is spawned.
    // The responsiveness parameter will be initialized here to maximum value
    // of 1.0, then depending on which action activation function is used,
    // the default undriven value may be changed to 1.0 or action midrange.
    void Indiv::initialize(uint16_t index_, Coord loc_, std::shared_ptr<Genome> genome_, Dir lastMove, unsigned dist)
    {
        index = index_;
        loc = loc_;
        //birthLoc = loc_;
        
        age = 0;
        oscPeriod = 34; // ToDo !!! define a constant
        alive = true;
        lastMoveDir = lastMove;
        responsiveness = 0.5; // range 0.0..1.0
        longProbeDist = dist;
        challengeBits = (unsigned)false; // will be set true when some task gets accomplished
        
        genome = genome_;

        createWiringFromGenome();
    }

    // This function is used when an agent is spawned. This function converts the
    // agent's inherited genome into the agent's neural net brain. There is a close
    // correspondence between the genome and the neural net, but a connection
    // specified in the genome will not be represented in the neural net if the
    // connection feeds a neuron that does not itself feed anything else.
    // Neurons get renumbered in the process:
    // 1. Create a set of referenced neuron numbers where each index is in the
    //    range 0..p.genomeMaxLength-1, keeping a count of outputs for each neuron.
    // 2. Delete any referenced neuron index that has no outputs or only feeds itself.
    // 3. Renumber the remaining neurons sequentially starting at 0.
    void Indiv::createWiringFromGenome()
    {
        NodeMap nodeMap;  // list of neurons and their number of inputs and outputs
        ConnectionList connectionList(p.maxNumberNeurons, nodeMap); // synaptic connections

        // Convert the indiv's genome to a renumbered connection list
        connectionList.renumber(genome);

        // Make a node (neuron) list from the renumbered connection list
        connectionList.makeNodeList();

        // Find and remove neurons that don't feed anything or only feed themself.
        // This reiteratively removes all connections to the useless neurons.
        connectionList.cullUselessNeurons();

        // The neurons map now has all the referenced neurons, their neuron numbers, and
        // the number of outputs for each neuron. Now we'll renumber the neurons
        // starting at zero.

        // remove dependency on global Params instance
        assert(nodeMap.size() <= p.maxNumberNeurons);

        uint16_t newNumber = 0;
        for (auto & node : nodeMap) {
            assert(node.second.numOutputs != 0);
            node.second.remappedNumber = newNumber++;
        }

        // Create the indiv's connection list in two passes:
        // First the connections to neurons, then the connections to actions.
        // This ordering optimizes the feed-forward function in feedForward.cpp.

        nnet.connections.clear();

        // First, the connections from sensor or neuron to a neuron
        for (auto const &conn : connectionList.connections()) {
            if (conn.sinkType == NEURON) {
                nnet.connections.push_back(conn);
                auto &newConn = nnet.connections.back();
                // fix the destination neuron number
                newConn.sinkNum = nodeMap[newConn.sinkNum].remappedNumber;
                // if the source is a neuron, fix its number too
                if (newConn.sourceType == NEURON) {
                    newConn.sourceNum = nodeMap[newConn.sourceNum].remappedNumber;
                }
            }
        }

        // Last, the connections from sensor or neuron to an action
        for (auto const &conn : connectionList.connections()) {
            if (conn.sinkType == ACTION) {
                nnet.connections.push_back(conn);
                auto &newConn = nnet.connections.back();
                // if the source is a neuron, fix its number
                if (newConn.sourceType == NEURON) {
                    newConn.sourceNum = nodeMap[newConn.sourceNum].remappedNumber;
                }
            }
        }

        // Create the indiv's neural node list
        nnet.neurons.clear();

        for (unsigned neuronNum = 0; neuronNum < nodeMap.size(); ++neuronNum) {
            nnet.neurons.push_back( {} );
            nnet.neurons.back().output = initialNeuronOutput();
            nnet.neurons.back().driven = (nodeMap[neuronNum].numInputsFromSensorsOrOtherNeurons != 0);
        }
    }
    

    float Indiv::distanceMoved() const
    {
        return (loc - birthLoc).length();
    }
    
} // end namespace BS

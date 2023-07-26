#include "../include/neuralNetBuilder.h"

namespace BS
{
    NeuralNetBuilder::NeuralNetBuilder(unsigned maxNeurons_) : maxNeurons(maxNeurons_){}

    /**
     * Construct a NNet from the Genome
    */
    std::shared_ptr<NeuralNet> NeuralNetBuilder::buildNeuralNet(std::shared_ptr<Genome> genome) const
    {
        // NodeMap nodeMap;  // list of neurons and their number of inputs and outputs
        ConnectionList connectionList(maxNeurons); // synaptic connections

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

        connectionList.remapSecondNodes(0);
        
        // const NodeMap &nodeMap = connectionList.getNodeMap();

        // assert(nodeMap.size() <= maxNeurons);

        // uint16_t newNumber = 0;

        // for (auto & node : nodeMap) {
        //     // TODO: make this assert redundant - cullUselessNeurons should ensure this
        //     assert(node.second.numOutputs != 0);
        //     node.second.remappedNumber = newNumber++;
        // }

        // Create the indiv's connection list in two passes:
        // First the connections to neurons, then the connections to actions.
        // This ordering optimizes the feed-forward function in feedForward.cpp.

        std::shared_ptr<NeuralNet> nnet = std::make_shared<NeuralNet>();

        nnet->create(connectionList.connections(), connectionList.getNodeMap());

        // TODO: this is redundant
        // nnet->connections.clear();

        // // First, the connections from sensor or neuron to a neuron
        // for (auto const &conn : connectionList.connections()) {
        //     if (conn.sinkType == NEURON) {
        //         // nnet->connections.push_back(conn);
        //         // auto &newConn = nnet->connections.back();
        //         auto &newConn = nnet->addConnection(conn);
        //         // fix the destination neuron number
        //         newConn.sinkNum = nodeMap[newConn.sinkNum].remappedNumber;
        //         // if the source is a neuron, fix its number too
        //         if (newConn.sourceType == NEURON) {
        //             newConn.sourceNum = nodeMap[newConn.sourceNum].remappedNumber;
        //         }
        //     }
        // }

        // // Last, the connections from sensor or neuron to an action
        // for (auto const &conn : connectionList.connections()) {
        //     if (conn.sinkType == ACTION) {
                
        //         // nnet->connections.push_back(conn);
        //         auto &newConn = nnet->addConnection(conn);
        //         // if the source is a neuron, fix its number
        //         if (newConn.sourceType == NEURON) {
        //             newConn.sourceNum = nodeMap[newConn.sourceNum].remappedNumber;
        //         }
        //     }
        // }

        // // Create the indiv's neural node list
        // // nnet->neurons.clear();

        // for (unsigned neuronNum = 0; neuronNum < nodeMap.size(); ++neuronNum) {
        //     nnet->neurons.push_back( {} );
        //     nnet->neurons.back().output = initialNeuronOutput();
        //     nnet->neurons.back().driven = (nodeMap[neuronNum].numInputsFromSensorsOrOtherNeurons != 0);
        // }

        return nnet;
    }

} // namespace BS

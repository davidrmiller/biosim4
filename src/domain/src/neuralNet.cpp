#include "../include/neuralNet.h"
#include "../include/node.h"

namespace BS
{
    void NeuralNet::create(std::list<Gene> genes, const NodeMap &nmap)
    {

        // first 2 steps create connections from the Genes & NodeMap
        connections.clear();

        for (auto const &conn : genes) {
            if (conn.sinkType == NEURON) {
                connections.push_back(conn);
                auto &newConn = connections.back();

                // auto &newConn = addConnection(conn);
                // fix the destination neuron number
                newConn.sinkNum = nmap.at(newConn.sinkNum).remappedNumber;

                // if the source is a neuron, fix its number too
                if (newConn.sourceType == NEURON) {
                    newConn.sourceNum = nmap.at(newConn.sourceNum).remappedNumber;
                }
            }
        }

        // Last, the connections from sensor or neuron to an action
        for (auto const &conn : genes) {
            if (conn.sinkType == ACTION) {
                
                connections.push_back(conn);
                auto &newConn = connections.back();

                // auto &newConn = addConnection(conn);
                // if the source is a neuron, fix its number
                if (newConn.sourceType == NEURON) {
                    newConn.sourceNum = nmap.at(newConn.sourceNum).remappedNumber;
                }
            }
        }

        // last step creates the neural node list from the nmap
        neurons.clear();

        for (unsigned neuronNum = 0; neuronNum < nmap.size(); ++neuronNum) {
            
            // just create the Neuron with the properties, why set them after instantiation?
            neurons.push_back( {} );
            neurons.back().output = initialNeuronOutput();
            neurons.back().driven = (nmap.at(neuronNum).numInputsFromSensorsOrOtherNeurons != 0);
        }

    }
    
    bool NeuralNet::hasConnections() const
    {
        return ! connections.empty();
    }

    const std::vector<Gene> &NeuralNet::getConnections() const
    {
        return connections;
    }

    Gene & NeuralNet::addConnection(Gene g)
    {
        connections.push_back(g);
        return connections.back();
    }

} // namespace BS

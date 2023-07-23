#include <cassert>

#include "../include/connectionList.h"

namespace BS
{
    ConnectionList::ConnectionList(unsigned max, NodeMap &map) : maxNeurons {max}, nodeMap{map} {}

    /**
     * Convert the indiv's genome to a renumbered connection list.
     * This renumbers the neurons from their uint16_t values in the genome
     * to the range 0..p.maxNumberNeurons - 1 by using a modulo operator.
     * Sensors are renumbered 0..Sensor::NUM_SENSES - 1
     * Actions are renumbered 0..Action::NUM_ACTIONS - 1
    */
    void ConnectionList::renumber(const Genome &genome)
    {
        connectionList.clear();

        for (auto const &gene : genome.genes()) {
            connectionList.push_back(gene);
            auto &conn = connectionList.back();

            if (conn.sourceType == NEURON) {
                conn.sourceNum %= maxNeurons;
            } else {
                conn.sourceNum %= Sensor::NUM_SENSES;
            }

            if (conn.sinkType == NEURON) {
                conn.sinkNum %= maxNeurons;
            } else {
                conn.sinkNum %= Action::NUM_ACTIONS;
            }
        }
    }

    /**
    * Scan the connections and make a list of all the neuron numbers
    * mentioned in the connections. Also keep track of how many inputs and
    * outputs each neuron has.
    */
    void ConnectionList::makeNodeList() const
    {
        nodeMap.clear();

        for (const Gene &conn : connectionList) {

            if (conn.sinkType == NEURON) {
                auto it = nodeMap.find(conn.sinkNum);
                if (it == nodeMap.end()) {
                    
                    assert(conn.sinkNum < maxNeurons);

                    nodeMap.insert(std::pair<uint16_t, Node>(conn.sinkNum, {} ));
                    it = nodeMap.find(conn.sinkNum);

                    assert(it->first < maxNeurons);
                    
                    it->second.numOutputs = 0;
                    it->second.numSelfInputs = 0;
                    it->second.numInputsFromSensorsOrOtherNeurons = 0;
                }

                if (conn.sourceType == NEURON && (conn.sourceNum == conn.sinkNum)) {
                    ++(it->second.numSelfInputs);
                } else {
                    ++(it->second.numInputsFromSensorsOrOtherNeurons);
                }
                assert(nodeMap.count(conn.sinkNum) == 1);
            }

            if (conn.sourceType == NEURON) {
                auto it = nodeMap.find(conn.sourceNum);
                if (it == nodeMap.end()) {
                    
                    assert(conn.sourceNum < maxNeurons);

                    nodeMap.insert(std::pair<uint16_t, Node>(conn.sourceNum, {} ));
                    it = nodeMap.find(conn.sourceNum);

                    assert(it->first < maxNeurons);
                    
                    it->second.numOutputs = 0;
                    it->second.numSelfInputs = 0;
                    it->second.numInputsFromSensorsOrOtherNeurons = 0;
                }
                ++(it->second.numOutputs);
                assert(nodeMap.count(conn.sourceNum) == 1);
            }
        }
    }

    /** 
     * During the culling process, we will remove any neuron that has no outputs,
     * and all the connections that feed the useless neuron.
    */
    void ConnectionList::removeConnectionsToNeuron(uint16_t neuronNumber)
    {
        for (auto itConn = connectionList.begin(); itConn != connectionList.end(); ) {
            if (itConn->sinkType == NEURON && itConn->sinkNum == neuronNumber) {
                // Remove the connection. If the connection source is from another
                // neuron, also decrement the other neuron's numOutputs:
                if (itConn->sourceType == NEURON) {
                    --(nodeMap[itConn->sourceNum].numOutputs);
                }
                itConn = connectionList.erase(itConn);
            } else {
                ++itConn;
            }
        }
    }


    /**
    * If a neuron has no outputs or only outputs that feed itself, then we
    * remove it along with all connections that feed it. Reiterative, because
    * after we remove a connection to a useless neuron, it may result in a
    * different neuron having no outputs.
    */
    void ConnectionList::cullUselessNeurons()
    {
        bool allDone = false;
        while (!allDone) {
            allDone = true;
            for (auto itNeuron = nodeMap.begin(); itNeuron != nodeMap.end(); ) {

                assert(itNeuron->first < maxNeurons);

                // We're looking for neurons with zero outputs, or neurons that feed itself
                // and nobody else:
                if (itNeuron->second.numOutputs == itNeuron->second.numSelfInputs) {  // could be 0
                    allDone = false;
                    // Find and remove connections from sensors or other neurons
                    removeConnectionsToNeuron(itNeuron->first);

                    itNeuron = nodeMap.erase(itNeuron);
                } else {
                    ++itNeuron;
                }
            }
        }
    }

    const ConnList &ConnectionList::connections()
    {
        return connectionList;
    }

} // namespace BS

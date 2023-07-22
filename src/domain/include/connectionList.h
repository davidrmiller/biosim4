#pragma once

#include <list>
#include <map>

#include "./node.h"
#include "./gene.h"
#include "../../sensors-actions.h"

namespace BS
{
    typedef std::map<uint16_t, Node> NodeMap;

    typedef std::list<Gene> ConnList;

    class ConnectionList {

        public:

            ConnectionList(unsigned maxNeurons, NodeMap &nodeMap);
            
            void renumber(const Genome &genome);
            void makeNodeList() const;
            void cullUselessNeurons();

            const ConnList &connections();

        private:
            unsigned maxNeurons = 300;

            ConnList connectionList;
            NodeMap &nodeMap;

            void removeConnectionsToNeuron(uint16_t neuronNumber);
    };

} // namespace BS

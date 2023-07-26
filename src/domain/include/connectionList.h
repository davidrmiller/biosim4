#pragma once

#include <list>
#include <map>

#include "./node.h"
#include "./gene.h"
#include "./genome.h"
#include "../../sensors-actions.h"

namespace BS
{
    // typedef std::map<uint16_t, Node> NodeMap;

    typedef std::list<Gene> ConnList;

    class ConnectionList {

        public:

            ConnectionList(unsigned maxNeurons);
            
            const NodeMap &getNodeMap() const;

            void renumber(const std::shared_ptr<Genome> genome);
            void makeNodeList();
            void cullUselessNeurons();
            void remapSecondNodes(uint16_t from);

            const ConnList &connections();

        private:
            unsigned maxNeurons = 300;

            ConnList connectionList;
            NodeMap nMap;

            void removeConnectionsToNeuron(uint16_t neuronNumber);
    };

} // namespace BS

// genome.cpp

#include <vector>
#include <map>
#include <list>
#include <iostream>
#include <cassert>
#include <string>
#include "simulator.h"
#include "random.h"
#include "genome-neurons.h"

#include "domain/include/gene.h"
#include "domain/include/connectionList.h"
#include "domain/include/node.h"

namespace BS {



// Two neuron renumberings occur: The original genome uses a uint16_t for
// neuron numbers. The first renumbering maps 16-bit unsigned neuron numbers
// to the range 0..p.maxNumberNeurons - 1. After culling useless neurons
// (see comments above), we'll renumber the remaining neurons sequentially
// starting at 0.
typedef std::map<uint16_t, Node> NodeMap; // key is neuron number 0..p.maxNumberNeurons - 1


// // Returns by value a single gene with random members.
// // See genome.h for the width of the members.
// // ToDo: don't assume the width of the members in gene.
// Gene makeRandomGene()
// {
//     Gene gene;

//     gene.sourceType = randomUint() & 1;
//     gene.sourceNum = (uint16_t)randomUint(0, 0x7fff);
//     gene.sinkType = randomUint() & 1;
//     gene.sinkNum = (uint16_t)randomUint(0, 0x7fff);
//     gene.weight =  randomUint(0, 0xffff) - 0x8000; //Gene::makeRandomWeight();


//     return gene;
// }


// // Returns by value a single genome with random genes.
// Genome makeRandomGenome()
// {
//     Genome genome;

//     unsigned length = randomUint(p.genomeInitialLengthMin, p.genomeInitialLengthMax);
//     for (unsigned n = 0; n < length; ++n) {
//         genome.push_back(makeRandomGene());
//     }

//     return genome;
// }



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


// ---------------------------------------------------------------------------


// This applies a point mutation at a random bit in a genome.
// void randomBitFlip(Genome &genome)
// {
//     int method = 1;

//     unsigned byteIndex = randomUint(0, genome.size() - 1) * sizeof(Gene);
//     unsigned elementIndex = randomUint(0, genome.size() - 1);
//     uint8_t bitIndex8 = 1 << randomUint(0, 7);

//     if (method == 0) {
//         ((uint8_t *)&genome[0])[byteIndex] ^= bitIndex8;
//     } else if (method == 1) {
//         float chance = randomUint() / (float)RANDOM_UINT_MAX; // 0..1
//         if (chance < 0.2) { // sourceType
//             genome[elementIndex].sourceType ^= 1;
//         } else if (chance < 0.4) { // sinkType
//             genome[elementIndex].sinkType ^= 1;
//         } else if (chance < 0.6) { // sourceNum
//             genome[elementIndex].sourceNum ^= bitIndex8;
//         } else if (chance < 0.8) { // sinkNum
//             genome[elementIndex].sinkNum ^= bitIndex8;
//         } else { // weight
//             genome[elementIndex].weight ^= (1 << randomUint(1, 15));
//         }
//     } else {
//         assert(false);
//     }
// }


// If the genome is longer than the prescribed length, and if it's longer
// than one gene, then we remove genes from the front or back. This is
// used only when the simulator is configured to allow genomes of
// unequal lengths during a simulation.
// void cropLength(Genome &genome, unsigned length)
// {
//     if (genome.size() > length && length > 0) {
//         if (randomUint() / (float)RANDOM_UINT_MAX < 0.5) {
//             // trim front
//             unsigned numberElementsToTrim = genome.size() - length;
//             genome.erase(genome.begin(), genome.begin() + numberElementsToTrim);
//         } else {
//             // trim back
//             genome.erase(genome.end() - (genome.size() - length), genome.end());
//         }
//     }
// }


// Inserts or removes a single gene from the genome. This is
// used only when the simulator is configured to allow genomes of
// unequal lengths during a simulation.
// void randomInsertDeletion(Genome &genome)
// {
//     float probability = p.geneInsertionDeletionRate;
//     if (randomUint() / (float)RANDOM_UINT_MAX < probability) {
//         if (randomUint() / (float)RANDOM_UINT_MAX < p.deletionRatio) {
//             // deletion
//             if (genome.size() > 1) {
//                 genome.erase(genome.begin() + randomUint(0, genome.size() - 1));
//             }
//         } else if (genome.size() < p.genomeMaxLength) {
//             // insertion
//             //genome.insert(genome.begin() + randomUint(0, genome.size() - 1), makeRandomGene());
//             genome.push_back(makeRandomGene());
//         }
//     }
// }


// This function causes point mutations in a genome with a probability defined
// by the parameter p.pointMutationRate.
// void applyPointMutations(Genome &genome)
// {
//     unsigned numberOfGenes = genome.size();
//     while (numberOfGenes-- > 0) {
//         if ((randomUint() / (float)RANDOM_UINT_MAX) < p.pointMutationRate) {
//             randomBitFlip(genome);
//         }
//     }
// }


// This generates a child genome from one or two parent genomes.
// If the parameter p.sexualReproduction is true, two parents contribute
// genes to the offspring. The new genome may undergo mutation.
// Must be called in single-thread mode between generations
// Genome generateChildGenome(const std::vector<Genome> &parentGenomes)
// {
//     // random parent (or parents if sexual reproduction) with random
//     // mutations
//     Genome genome;

//     uint16_t parent1Idx;
//     uint16_t parent2Idx;

//     // Choose two parents randomly from the candidates. If the parameter
//     // p.chooseParentsByFitness is false, then we choose at random from
//     // all the candidate parents with equal preference. If the parameter is
//     // true, then we give preference to candidate parents according to their
//     // score. Their score was computed by the survival/selection algorithm
//     // in survival-criteria.cpp.
//     if (p.chooseParentsByFitness && parentGenomes.size() > 1) {
//         parent1Idx = randomUint(1, parentGenomes.size() - 1);
//         parent2Idx = randomUint(0, parent1Idx - 1);
//     } else {
//         parent1Idx = randomUint(0, parentGenomes.size() - 1);
//         parent2Idx = randomUint(0, parentGenomes.size() - 1);
//     }

//     const Genome &g1 = parentGenomes[parent1Idx];
//     const Genome &g2 = parentGenomes[parent2Idx];

//     if (g1.empty() || g2.empty()) {
//         std::cout << "invalid genome" << std::endl;
//         assert(false);
//     }

//     auto overlayWithSliceOf = [&](const Genome &gShorter) {
//         uint16_t index0 = randomUint(0, gShorter.size() - 1);
//         uint16_t index1 = randomUint(0, gShorter.size());
//         if (index0 > index1) {
//             std::swap(index0, index1);
//         }
//         std::copy(gShorter.begin() + index0, gShorter.begin() + index1, genome.begin() + index0);
//     };

//     if (p.sexualReproduction) {
//         if (g1.size() > g2.size()) {
//             genome = g1;
//             overlayWithSliceOf(g2);
//             assert(!genome.empty());
//         } else {
//             genome = g2;
//             overlayWithSliceOf(g1);
//             assert(!genome.empty());
//         }

//         // Trim to length = average length of parents
//         unsigned sum = g1.size() + g2.size();
//         // If average length is not an integral number, add one half the time
//         if ((sum & 1) && (randomUint() & 1)) {
//             ++sum;
//         }
//         cropLength(genome, sum / 2);
//         assert(!genome.empty());
//     } else {
//         genome = g2;
//         assert(!genome.empty());
//     }

//     randomInsertDeletion(genome);
//     assert(!genome.empty());
//     applyPointMutations(genome);
//     assert(!genome.empty());
//     assert(genome.size() <= p.genomeMaxLength);

//     return genome;
// }

} // end namespace BS

#ifndef GENOME_H_INCLUDED
#define GENOME_H_INCLUDED

#include <cstdint>
#include <vector>
#include <cmath>
#include "sensors-actions.h"
#include "random.h"
#include "domain/include/gene.h"
namespace BS {


// When a new population is generated and every individual is given a
// neural net, the neuron outputs must be initialized to something:
//constexpr float initialNeuronOutput() { return (NEURON_RANGE / 2.0) + NEURON_MIN; }
constexpr float initialNeuronOutput() { return 0.5; }

// extern Gene makeRandomGene();
// extern Genome makeRandomGenome();
extern void unitTestConnectNeuralNetWiringFromGenome();
extern float genomeSimilarity(const Genome &g1, const Genome &g2); // 0.0..1.0
extern float geneticDiversity();  // 0.0..1.0

} // end namespace BS

#endif // GENOME_H_INCLUDED

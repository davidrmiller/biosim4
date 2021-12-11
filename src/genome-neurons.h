#ifndef GENOME_H_INCLUDED
#define GENOME_H_INCLUDED

#include <cstdint>
#include <vector>
#include <cmath>
#include "sensors-actions.h"
#include "random.h"

namespace BS {

// Each gene specifies one synaptic connection in a neural net. Each
// connection has an input (source) which is either a sensor or another neuron.
// Each connection has an output, which is either an action or another neuron.
// Each connection has a floating point weight derived from a signed 16-bit
// value. The signed integer weight is scaled to a small range, then cubed
// to provide fine resolution near zero.

constexpr uint8_t SENSOR = 1;  // always a source
constexpr uint8_t ACTION = 1;  // always a sink
constexpr uint8_t NEURON = 0;  // can be either a source or sink

struct Gene { //__attribute__((packed)) Gene {
    uint16_t sourceType:1; // SENSOR or NEURON
    uint16_t sourceNum:7;
    uint16_t sinkType:1;   // NEURON or ACTION
    uint16_t sinkNum:7;
    int16_t weight;

    static constexpr float f1 = 8.0;
    static constexpr float f2 = 64.0;
    //float weightAsFloat() { return std::pow(weight / f1, 3.0) / f2; }
    float weightAsFloat() const { return weight / 8192.0; }
    static int16_t makeRandomWeight() { return randomUint(0, 0xffff) - 0x8000; }
};


// An individual's genome is a set of Genes (see Gene comments above). Each
// gene is equivalent to one connection in a neural net. An individual's
// neural net is derived from its set of genes.
typedef std::vector<Gene> Genome;


// An individual's "brain" is a neural net specified by a set
// of Genes where each Gene specifies one connection in the neural net (see
// Genome comments above). Each neuron has a single output which is
// connected to a set of sinks where each sink is either an action output
// or another neuron. Each neuron has a set of input sources where each
// source is either a sensor or another neuron. There is no concept of
// layers in the net: it's a free-for-all topology with forward, backwards,
// and sideways connection allowed. Weighted connections are allowed
// directly from any source to any action.

// Currently the genome does not specify the activation function used in
// the neurons. (May be hardcoded to std::tanh() !!!)

// When the input is a sensor, the input value to the sink is the raw
// sensor value of type float and depends on the sensor. If the output
// is an action, the source's output value is interpreted by the action
// node and whether the action occurs or not depends on the action's
// implementation.

// In the genome, neurons are identified by 15-bit unsigned indices,
// which are reinterpreted as values in the range 0..p.genomeMaxLength-1
// by taking the 15-bit index modulo the max number of allowed neurons.
// In the neural net, the neurons that end up connected get new indices
// assigned sequentially starting at 0.


struct NeuralNet {
    std::vector<Gene> connections; // connections are equivalent to genes

    struct Neuron {
        float output;
        bool driven;        // undriven neurons have fixed output values
    };
    std::vector<Neuron> neurons;
};

// When a new population is generated and every individual is given a
// neural net, the neuron outputs must be initialized to something:
//constexpr float initialNeuronOutput() { return (NEURON_RANGE / 2.0) + NEURON_MIN; }
constexpr float initialNeuronOutput() { return 0.5; }

extern Gene makeRandomGene();
extern Genome makeRandomGenome();
extern void unitTestConnectNeuralNetWiringFromGenome();
extern float genomeSimilarity(const Genome &g1, const Genome &g2); // 0.0..1.0
extern float geneticDiversity();  // 0.0..1.0

} // end namespace BS

#endif // GENOME_H_INCLUDED

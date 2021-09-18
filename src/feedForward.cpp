// feedForward.cpp - reads sensors, returns actions, for an individual

#include <iostream>
#include <cassert>
#include <cmath>
#include "simulator.h"

namespace BS {

/********************************************************************************
This function does a neural net feed-forward operation, from sensor (input) neurons
through internal neurons to action (output) neurons. The feed-forward
calculations are evaluated once each simulator step (simStep).

There is no back-propagation in this simulator. Once an individual's neural net
brain is wired at birth, the weights and topology do not change during the
individual's lifetime.

The data structure Indiv::neurons contains internal neurons, and Indiv::connections
holds the connections between the neurons.

We have three types of neurons:

     input sensors - each gives a value in the range SENSOR_MIN.. SENSOR_MAX (0.0..1.0).
         Values are obtained from getSensor().

     internal neurons - each takes inputs from sensors or other internal neurons;
         each has output value in the range NEURON_MIN..NEURON_MAX (-1.0..1.0). The
         output value for each neuron is stored in Indiv::neurons[] and survives from
         one simStep to the next. (For example, a neuron that feeds itself will use
         its output value that was latched from the previous simStep.) Inputs to the
         neurons are summed each simStep in a temporary container and then discarded
         after the neurons' outputs are computed.

     action (output) neurons - each takes inputs from sensors or other internal
         neurons; In this function, each has an output value in an arbitrary range
         (because they are the raw sums of zero or more weighted inputs).
         The values of the action neurons are saved in local container
         actionLevels[] which is returned to the caller by value (thanks RVO).
********************************************************************************/

std::array<float, Action::NUM_ACTIONS> Indiv::feedForward(unsigned simStep)
{
    // This container is used to return values for all the action outputs. This array
    // contains one value per action neuron, which is the sum of all its weighted
    // input connections. The sum has an arbitrary range. Return by value assumes compiler
    // return value optimization.
    std::array<float, Action::NUM_ACTIONS> actionLevels;
    actionLevels.fill(0.0); // undriven actions default to value 0.0

    // Weighted inputs to each neuron are summed in neuronAccumulators[]
    std::vector<float> neuronAccumulators(nnet.neurons.size(), 0.0);

    // Connections were ordered at birth so that all connections to neurons get
    // processed here before any connections to actions. As soon as we encounter the
    // first connection to an action, we'll pass all the neuron input accumulators
    // through a transfer function and update the neuron outputs in the indiv,
    // except for undriven neurons which act as bias feeds and don't change. The
    // transfer function will leave each neuron's output in the range -1.0..1.0.

    bool neuronOutputsComputed = false;
    for (Gene & conn : nnet.connections) {
        if (conn.sinkType == ACTION && !neuronOutputsComputed) {
            // We've handled all the connections from sensors and now we are about to
            // start on the connections to the action outputs, so now it's time to
            // update and latch all the neuron outputs to their proper range (-1.0..1.0)
            for (unsigned neuronIndex = 0; neuronIndex < nnet.neurons.size(); ++neuronIndex) {
                if (nnet.neurons[neuronIndex].driven) {
                    nnet.neurons[neuronIndex].output = std::tanh(neuronAccumulators[neuronIndex]);
                }
            }
            neuronOutputsComputed = true;
        }

        // Obtain the connection's input value from a sensor neuron or other neuron
        // The values are summed for now, later passed through a transfer function
        float inputVal;
        if (conn.sourceType == SENSOR) {
            inputVal = getSensor((Sensor)conn.sourceNum, simStep);
        } else {
            inputVal = nnet.neurons[conn.sourceNum].output;
        }

        // Weight the connection's value and add to neuron accumulator or action accumulator.
        // The action and neuron accumulators will therefore contain +- float values in
        // an arbitrary range.
        if (conn.sinkType == ACTION) {
            actionLevels[conn.sinkNum] += inputVal * conn.weightAsFloat();
        } else {
            neuronAccumulators[conn.sinkNum] += inputVal * conn.weightAsFloat();
        }
    }

    return actionLevels;
}

} // end namespace BS


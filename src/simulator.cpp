// simulator.cpp - Main thread

// This file contains simulator(), the top-level entry point of the simulator.
// simulator() is called from main.cpp with a copy of argc and argv.
// If there is no command line argument, the simulator will read the default
// config file ("biosim4.ini" in the current directory) to get the simulation
// parameters for this run. If there are one or more command line args, then
// argv[1] must contain the name of the config file which will be read instead
// of biosim4.ini. Any args after that are ignored. The simulator code is
// in namespace BS (for "biosim").

#include <iostream>
#include <chrono>
#include <cassert>
#include <utility>
#include <algorithm>
#include "simulator.h"     // the simulator data structures

namespace BS {

extern void initializeGeneration0();
extern void initializeFromSave();
extern unsigned spawnNewGeneration(unsigned generation, unsigned murderCount);
extern void displaySampleGenomes(unsigned count);
extern void executeActions(Indiv &indiv, std::array<float, Action::NUM_ACTIONS> &actionLevels);
extern void endOfSimStep(unsigned simStep, unsigned generation);

RunMode runMode = RunMode::STOP;
Grid grid;        // The 2D world where the creatures live
Signals signals;  // A 2D array of pheromones that overlay the world grid
Peeps peeps;      // The container of all the individuals in the population

UserIO* userIO;

// The paramManager maintains a private copy of the parameter values, and a copy
// is available read-only through global variable p. Although this is not
// foolproof, you should be able to modify the config file during a simulation
// run and modify many of the parameters. See params.cpp and params.h for more info.
ParamManager paramManager;
const Params &p { paramManager.getParamRef() }; // read-only params


/**********************************************************************************************
Execute one simStep for one individual.

This executes in its own thread, invoked from the main simulator thread. First we execute
indiv.feedForward() which computes action values to be executed here. Some actions such as
signal emission(s) (pheromones), agent movement, or deaths will have been queued for
later execution at the end of the generation in single-threaded mode (the deferred queues
allow the main data structures (e.g., grid, signals) to be freely accessed read-only in all threads).

In order to be thread-safe, the main simulator-wide data structures and their
accessibility are:

    grid - read-only
    signals - (pheromones) read-write for the location where our agent lives
        using signals.increment(), read-only for other locations
    peeps - for other individuals, we can only read their index and genome.
        We have read-write access to our individual through the indiv argument.

The other important variables are:

    simStep - the current age of our agent, reset to 0 at the start of each generation.
         For many simulation scenarios, this matches our indiv.age member.
    randomUint - global random number generator, a private instance is given to each thread
**********************************************************************************************/
void simStepOneIndiv(Indiv &indiv, unsigned simStep)
{
    ++indiv.age; // for this implementation, tracks simStep
    auto actionLevels = indiv.feedForward(simStep);
    executeActions(indiv, actionLevels);
}

void simulate(unsigned generation)
{
    unsigned murderCount;

    // Inside the parallel region, be sure that shared data is not modified. Do the
    // modifications in the single-thread regions.
    #pragma omp parallel num_threads(p.numThreads) default(shared)
    {
        randomUint.initialize(); // seed the RNG, each thread has a private instance

        while (runMode == RunMode::RUN && generation < p.maxGenerations && !userIO->isStopped())
        { // generation loop
            #pragma omp master
            {
                murderCount = 0; // for reporting purposes
                userIO->startNewGeneration(generation, p.stepsPerGeneration);
            }
            #pragma omp barrier

            for (unsigned simStep = 0; simStep < p.stepsPerGeneration && !userIO->isStopped() && runMode == RunMode::RUN; ++simStep)
            {
                // multithreaded loop: index 0 is reserved, start at 1
                #pragma omp for schedule(auto)
                for (unsigned indivIndex = 1; indivIndex <= p.population; ++indivIndex)
                {
                    if (peeps[indivIndex].alive)
                    {
                        simStepOneIndiv(peeps[indivIndex], simStep);
                    }
                }

                // In single-thread mode: this executes deferred, queued deaths and movements,
                // updates signal layers (pheromone), etc.
                // UPDATED: rendering must be done in the master thread
                #pragma omp master
                {
                    murderCount += peeps.deathQueueSize();
                    endOfSimStep(simStep, generation);

                    userIO->handleStep(simStep, generation);

                    if (userIO->getLoadFileSelected())
                    {
                        runMode = RunMode::LOAD;
                    }
                }

                #pragma omp barrier
            }

            #pragma omp master
            {
                if (runMode == RunMode::RUN) {
                    userIO->endOfGeneration(generation);

                    // ToDo: make it work alongside with updateFromUi
                    // paramManager.updateFromConfigFile(generation + 1);

                    paramManager.updateFromUi();
                    unsigned numberSurvivors = spawnNewGeneration(generation, murderCount);
                    if (numberSurvivors > 0 && (generation % p.genomeAnalysisStride == 0))
                    {
                        // displaySampleGenomes(p.displaySampleGenomes);
                    }
                    if (numberSurvivors == 0)
                    {
                        generation = 0; // start over
                    }
                    else
                    {
                        ++generation;
                    }
                }
            }
            #pragma omp barrier
        }
    }

    if (runMode == RunMode::RUN) {
        runMode = RunMode::STOP;
    }
    // displaySampleGenomes(3); // final report, for debugging

    std::cout << "Simulator exit." << std::endl;
}

/********************************************************************************
Start of simulator

All the agents are randomly placed with random genomes at the start. The outer
loop is generation, the inner loop is simStep. There is a fixed number of
simSteps in each generation. Agents can die at any simStep and their corpses
remain until the end of the generation. At the end of the generation, the
dead corpses are removed, the survivors reproduce and then die. The newborns
are placed at random locations, signals (pheromones) are updated, simStep is
reset to 0, and a new generation proceeds.

The paramManager manages all the simulator parameters. It starts with defaults,
then keeps them updated as the config file (biosim4.ini) changes.

The main simulator-wide data structures are:
    grid - where the agents live (identified by their non-zero index). 0 means empty.
    signals - multiple layers overlay the grid, hold pheromones
    peeps - an indexed set of agents of type Indiv; indexes start at 1

The important simulator-wide variables are:
    generation - starts at 0, then increments every time the agents die and reproduce.
    simStep - reset to 0 at the start of each generation; fixed number per generation.
    randomUint - global random number generator

The threads are:
    main thread - simulator
    simStepOneIndiv() - child threads created by the main simulator thread
********************************************************************************/
void simulator(int argc, char **argv)
{
    //printSensorsActions(); // show the agents' capabilities

    // Simulator parameters are available read-only through the global
    // variable p after paramManager is initialized.
    // Todo: remove the hardcoded parameter filename.
    paramManager.setDefaults();
    paramManager.registerConfigFile(argc > 1 ? argv[1] : "biosim4.ini");
    paramManager.updateFromConfigFile(0);
    paramManager.checkParameters(); // check and report any problems

    randomUint.initialize(); // seed the RNG for main-thread use

    // UI must be initialized after parameters
    userIO = new UserIO(true, false);
    
    runMode = RunMode::RUN;
    unsigned generation = 0;

    while (runMode != RunMode::STOP) {
        switch (runMode)
        {
        case RunMode::RUN:
            // Allocate container space. Once allocated, these container elements
            // will be reused in each new generation.
            grid.init(p.sizeX, p.sizeY);                    // the land on which the peeps live
            signals.init(p.signalLayers, p.sizeX, p.sizeY); // where the pheromones waft
            peeps.init(p.population);                       // the peeps themselves

            generation = 0;
            
            initializeGeneration0(); // starting population
            simulate(generation);
            break;
        case RunMode::LOAD:
            grid.init(p.sizeX, p.sizeY);
            signals.init(p.signalLayers, p.sizeX, p.sizeY);

            generation = 0;

            Save::load(userIO->getLoadFilename());
            userIO->setFromParams();
            peeps.initFromSave();
            initializeFromSave();
            userIO->cleanLoadSelection();

            runMode = RunMode::RUN;
            simulate(generation);
            break;
        default:
            break;
        }        
    }

    delete userIO;
}

} // end namespace BS
# Environment class: the bounded context of the world
    * An instance of the Domain
    * Entry point to access an Environment's objects
    * Has a Grid + layers
    * Has a population (Peeps instance)
    * Peeps has Indivs
    * Indivs have a Genome & a NNet
    * Can be reset? or just create a new instance from (part of) the Genome of another Environment?
    * Can be presented

# Experiment class: runs the steps

    * main.cpp 
        * create experiment
        * then run it

        * startExperiment
            * startGeneration
                Environment::executeSteps(unsigned num);
            * endGeneration
                Environment::respawn
        * endExperiment - event


    * Support extensions of each Experiment step?
    
# Experiment
    * Simulator as a class
    * creates Environments according to its requirements
        * Grid size
        * Layers
        * Num individuals
        * Num genes / neurons
        * etc
    * runs experiments by calling
        * Environment::executeSteps(unsigned num);
        * then Environment::respawn() or similar

# Environment


* Experiment has 1+ Environments
* Environment has a Grid
* Environment has layers
* Environment has a Population (peeps)
* Population has Individuals
* Individual has a Genome & a NeuralNet contructed from its Genome
* Genome has Genes
* NeuralNet has Neurons (& Genes - why??)


# Genes, NeuralNets

* A Genome contains Genes (not necessarily useful)
* Start with a Genome containing a random set of Genes created by a GenomeBuilder
* Call a NeuralNetBuilder to build a NeuralNet from the Genome
* NeuralNetBuilder instructs a ConnectionList to create the NeuralNet, all the NNB does is call CList methods in order
* ConnectionList:
    * copies & renumbers the Genes
    * makes a NodeList (nodeMap std::map<uint16_t, Node>) from the copied Genes
    * removes useless Genes from its copy of the Genome
    * remaps the Nodes in the NodeMap, sets their remappedNumber

* NeuralNetBuilder calls a NeuralNet to create itself with the connections (Genes) and NodeMap from the ConnectionList
    * Use the NNet constructor?

* NeuralNet:
    * copies the Genes into its own vector of genes (connections)
    * its sets the sourceNum & sinkNum of the Genes
    * creates its Neurons from the NodeMap

* Then an Indiv is initialized with:
    * The original Genome from the GenomeBuilder
    * The NeuralNet built from that genome

* Then the application loop
    * For the configure number of generations
        * For the configured number of steps per generation
            * actionLevels = Indiv.feedForward(step)
            * executeActions(indiv, actionLevels)
        * Cull Indivs
        * Respawn Genomes, NNets & reinit Indivs
    


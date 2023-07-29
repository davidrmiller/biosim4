[x] testing? Catch2? 

[] use the Grid instance as the source of world size, don't refer to the parameters x & y apart from instantiating the grid

[] remove dependencies on globals - eg params instance

[x] move the static Dir::random8() method to a factory method - out of the Dir class

    * pass the dir instance to the Indiv::initialise() method
        * instead of it calling Dir::random8()
    
    * executeActions() Action::MOVE_RANDOM
        * use the dirRandom8() factory method
        * DONE

# four layers, proposal

# common contains stand-alone classes not specific to this domain
* code to interact with a 2d world

    * Compass
    * Coord(inate)
    * Dir(ection)
    * Polar (coordinate)
    * Grid? is this just a container?
        * move global functions in getSensor.cpp to Grid class
        * createBarrier(Grid g, barrier_type) - move to a grid builder class 
        
        * move visitNeighborhood to a GridVisitor class & interface
            * https://refactoring.guru/design-patterns/visitor?
            * GridVisitor v = BarrierCreatorVisitor(Grid g)

            * Grid::accept(GridVisitor v)
                * iterate through Coordinates calling
                    * visitor.visit(Coord loc)
        * or a Grid iterator to iterate over points on the grid

        * add layers to the Grid data type? - for use by signals
            * Layer struct in signals is a variety of Grid

        * add an explicit Grid::setBarrier(Coord loc) method


    * RandomUintGenerator

    [] Add Point class, simple x & y locations


# domain contains classes that form the domain model

* Declare class responsibilities and the domain model first
* Find other domain models hidden in the procedural code

    * Indiv(idual) - rename to full name?
        * convert to a class
        * create a IndivBuilder class
        * move feedForward method to indiv.cpp
        * Indiv::getSensor method to Peeps class?
        * Indiv::createWiringFromGenome - move to a builder / factory?
        * move Indiv::feedForward to be a NeuralNet class method?

    * Peeps - rename to Population?
        * make into a wrapper around grid, pass grid instance to constructor
        * change
        if (grid.isInBounds(otherLoc) && grid.isOccupiedAt(otherLoc)) {
                Indiv &indiv2 = peeps.getIndiv(otherLoc);
        }

        * to
            if peeps.hasIndiv(loc){
                Indiv &indiv2 = peeps.getIndiv(loc);
            }
        
    * Gene 
    * Genome - [x] convert to a class
    * Signals
    * Sensor
    * Actions
    * genome-compare.cpp
    * factory / builders of the domain

* move executeActions() to be an Indiv class method

* Add an Environment class to contain the Grid, Peeps etc
    * Use this to operate on Indivs in the environment eg getSensor()

# application
* classes / code
    * Simulator class
    * endOfSimStep.cpp
    * endOfGeneration.cpp
    * executeActions.cpp
    * main.cpp
    * Params class
    * spawnNewGeneration.cpp
        * convert to a world builder class - build grid, creates a population 
    * survival-criteria.cpp?? 
        * move the grid logic into Grid class, eg is a location inside a Grid area
        * the application code is the parts that decide how to calculate if an individual survives
    

* uses the domain for a specific application / demonstration of the domain model
* instantiates the domain
* uses it's parameters to build the world

# storage
* save & load state?

# presentation
* Text 
* image file
* video file
* real-time

* code / classes    
    * analysis.cpp
    * imageWriter.cpp - decouple from the biosim application
    * image / video generation


# Gene 
* convert to a class? add more methods at least

* move Gene::makeRandomWeight to a Gene factory class, it's only used by makeRandomGene()
* move makeRandomGenome() to Genome or Gene factory too

* move genesMatch to Gene == operator?
* move global Gene comparison methods to Gene 

    * jaro_winkler_distance(g1, g2);
    * hammingDistanceBits(g1, g2);
    * hammingDistanceBytes(g1, g2);

* move genomeSimilarity to Gene - remove the global p

* move geneticDiversity to peeps 

# genome.cpp contains global procedural functions to operate on the Genome
* make Genome into a class with this methods

# Peeps & Grid

* think about how Peeps interacts with Grid and Indiv
* also how an Indiv behaviour is a combination of its position on the grid, its neural network & the pheremones, Indiv uses global Grid instance, should it have a reference?

* grid is used directly to understand the game area

* spawnNewGeneration should be a Peeps method? or a builder class method?

* Peeps::drainMoveQueue


* rewrite 'Indiv move' as a Grid method? moving individuals must then be declared as a responsibility of Grid
    * Need to move Grid out of Common into the Domain 
    * Is this an Environment method? env->moveIndiv(I, C)?

bool Grid::moveIndiv(Indiv i, Coord newLoc)
{
    if (isEmptyAt(newLoc)) {
        Dir moveDir = (newLoc - indiv.loc).asDir();
        set(indiv.loc, 0);
        set(newLoc, indiv.index);
        indiv.loc = newLoc;
        indiv.lastMoveDir = moveDir;
        return true;
    } else {
        return false;
    }

}

# Questions

[] How much has the presentation of the simulation affected the design of the domain?
[] The Grid is the environment of the experiment, as such it's part of the domain, since the experiment takes place in an environment of Individuals with their neighbors
* 

# Additions

[] Multiple Gene containers exist - why?

    * Genome std::vector<Gene> genome;
        - the full Genome (produced randomly or from sexual reproduction)

    * ConnectionList std::list<Gene> list; 
        - the Genes from a Genome with useless Genes removed

    * NeuralNet std::vector<Gene> connections; 
        - from a ConnectionList's set of Genes, reordered by sinkType

[] Idea: Genome has the set of Genomes and the NodeMap
    * remove the need for Connection List
    * Genome creates the NeuralNet based on its Genes

[] Genome & other classes need to know about the parameter maxNeurons
    * Set a static class variable on Genome? 
    * as part of environment initialization?
    * it's only used by ConnectionList for various validations of Genes

[] Move Sensor & Action enums into Gene class

# Sensors & getSensor?

[] getSensor uses the Gene (of the Nnet connections) not the NNet - shouldn't behaviour be based on the NNet not the Gene? or the interaction with the environment be something the Indiv does?

[] consider sensor objects instead of values in a Gene
* indiv.getSensor() becomes gene.sensor.getValue(&indiv, simStep)?
* and gene.getActionValue(&indiv)?

* sensors use flyweight pattern?
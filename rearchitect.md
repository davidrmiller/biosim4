* use the Grid instance as the source of world size, don't refer to the parameters x & y apart from instantiating the grid

* remove dependencies on globals - eg params instance

* move the static Dir::random8() method to a factory method - out of the Dir class

    * pass the dir instance to the Indiv::initialise() method
        * instead of it calling Dir::random8()
    
    * executeActions() Action::MOVE_RANDOM
        * use the dirRandom8() factory method
        * DONE

* move create barrier method definition into grid.cpp


# four layers, proposal

# common contains stand-alone classes not specific to this domain
* code to interact with a 2d world

    * Compass
    * Coord(inate)
    * Dir(ection)
    * Polar (coordinate)
    * Grid
        * move global functions in getSensor.cpp to Grid class
        * createBarrier(Grid g, barrier_type) - move to a grid builder class? 
        * move visitNeighborhood to Grid class?

    * RandomUintGenerator

# domain contains classes that form the domain model

* Declare class responsibilities and the domain model first
* Find other domain models hidden in the procedural code

    * Indiv(idual) - rename to full name?
        * convert to a class

        * move feedForward method to indiv.cpp
        * Indiv::getSensor method to Peeps class?

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
    * Genome - convert to a class
    * Signals
    * Sensor
    * Actions
    * genome-compare.cpp
    * factory / builders of the domain

* move executeActions() to be an Indiv class method
* move Indiv::feedForward to be a NeuralNet class method?

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
* grid is used directly to understand the game area

* spawnNewGeneration should be a Peeps method?

* Peeps::drainMoveQueue


* rewrite 'Indiv move' as a Grid method? moving individuals must then be declared as a responsibility of Grid

Grid::moveIndiv(Indiv i, Coord newLoc)
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

        
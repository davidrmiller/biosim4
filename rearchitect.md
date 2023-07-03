* use the Grid instance as the source of world size, don't refer to the parameters x & y apart from instantiating the grid

* remove dependencies on globals - eg params instance

* move the static Dir::random8() method to a factory method - out of the Dir class
    * temporarily to a global factory function?

    * pass the dir instance to the Indiv::initialise() method
        * instead of it calling Dir::random8()
    
    * executeActions() Action::MOVE_RANDOM
        * use the dirRandom8() factory method
    

* move Grid to the common lib? or domain?

* common lib contains stand-alone classes not specific to this domain
    * compass
    * coord(inate)
    * dir(ection)
    * polar (coordinate)
    * grid?

* domain lib contains classes that form the domain model
    * Individual - rename?
    * peeps - rename to population
    * grid? 
    * genome
    * signals
    * sensor
    * actions

#include <iostream>

// This is included here only for the purpose of unit testing of basic types
#include "basicTypes.h"

// This is the only call needed to start the simulator. No header file
// because it's just one declaration. Pass it argc and argv as if from main().
// If there is no command line argument, the simulator will read the default
// config file ("biosim4.ini" in the current directory) to get the simulation
// parameters for this run. If there are one or more command line args, then
// argv[1] must contain the name of the config file which will be read instead
// of biosim4.ini. Any args after that are ignored. The simulator code is
// in namespace BS (for "biosim").
namespace BS {
    void simulator(int argc, char **argv);
}


int main(int argc, char **argv)
{
    BS::unitTestBasicTypes(); // called only for unit testing of basic types

    // Start the simulator with optional config filename (default "biosim4.ini").
    // See simulator.cpp and simulator.h.
    BS::simulator(argc, argv); 

    return 0;
}

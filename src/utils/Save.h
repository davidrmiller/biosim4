#ifndef SAVE_H_INCLUDED
#define SAVE_H_INCLUDED

#include <iostream>
#include <cstdint>
#include <vector>
#include <cereal/access.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <fstream>

namespace BS {

// This class keeps track of alive and dead Indiv's and where they
// are in the Grid.
// Peeps allows spawning a live Indiv at a random or specific location
// in the grid, moving Indiv's from one grid location to another, and
// killing any Indiv.
// All the Indiv instances, living and dead, are stored in the private
// .individuals member. The .cull() function will remove dead members and
// replace their slots in the .individuals container with living members
// from the end of the container for compacting the container.
// Each Indiv has an identifying index in the range 1..0xfffe that is
// stored in the Grid at the location where the Indiv resides, such that
// a Grid element value n refers to .individuals[n]. Index value 0 is
// reserved, i.e., .individuals[0] is not a valid individual.
// This class does not manage properties inside Indiv except for the
// Indiv's location in the grid and its aliveness.
class Save {
public:
    static void save(std::string fileName = "Output/Saves/simulation.bin");
    static void load(std::string fileName = "Output/Saves/simulation.bin");
};

} // end namespace BS

#endif // SAVE_H_INCLUDED

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

/**
 * Class for saving and loading simulations in binary format
 * and saving neural network images
 */
class Save {
public:
    static void save(std::string fileName = "Output/Saves/simulation.bin");
    static void load(std::string fileName = "Output/Saves/simulation.bin");
    static std::string saveNet(int selectedIndex);
};

} // end namespace BS

#endif // SAVE_H_INCLUDED

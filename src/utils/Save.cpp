#include "Save.h"
#include "../simulator.h"

namespace BS
{
    /**
     * Saves simulation in binary format using cereal library
     * see void serialize(Archive &ar) function in class Peeps and class Params
     */
    void Save::save(std::string fileName)
    {
        std::ofstream file(fileName);
        cereal::BinaryOutputArchive archive(file);
        Params newParams;
        archive(peeps, p);
    }

    /**
     * Loads simulation from binary file into peeps and Params
     */
    void Save::load(std::string fileName)
    {
        std::ifstream file(fileName);
        cereal::BinaryInputArchive archive(file);        
        Params newParams;
        archive(peeps, newParams);
        paramManager.updateFromSave(newParams);
    }

    /**
     * Saves neural network image using printIGraphEdgeList of indiv
     * and graph-nnet.py script
     */
    std::string Save::saveNet(int selectedIndex)
    {
        std::ofstream outFile;
        outFile.open("tools/net.txt");   
        outFile << peeps[selectedIndex].printIGraphEdgeList().rdbuf();
        outFile.close();
        std::string filename = "Output/Images/indiv-" + std::to_string(selectedIndex) + ".svg";
        std::string command = "python3 ./tools/graph-nnet.py -o " + filename;
        std::system(command.c_str());
        return filename;
    }
}
#include "Save.h"
#include "../simulator.h"

namespace BS
{
    void Save::save(std::string fileName)
    {
        // ToDo: flip to binary
        std::ofstream file(fileName);
        cereal::BinaryOutputArchive archive(file);
        Params newParams;
        archive(peeps, p);
    }

    void Save::load(std::string fileName)
    {
        // ToDo: flip to binary
        std::ifstream file(fileName);
        cereal::BinaryInputArchive archive(file);        
        Params newParams;
        archive(peeps, newParams);
        paramManager.updateFromSave(newParams);
    }
}
#include "Save.h"
#include "../simulator.h"

namespace BS
{
    void Save::save(std::string fileName)
    {
        std::ofstream file(fileName);
        cereal::BinaryOutputArchive archive(file);
        Params newParams;
        archive(peeps, p);
    }

    void Save::load(std::string fileName)
    {
        std::ifstream file(fileName);
        cereal::BinaryInputArchive archive(file);        
        Params newParams;
        archive(peeps, newParams);
        paramManager.updateFromSave(newParams);
    }
}
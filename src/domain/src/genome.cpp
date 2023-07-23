#include <cassert>

#include "../include/genome.h"

namespace BS
{
    /**
     * Adds a new Gene
    */
    void Genome::add(Gene g) 
    {
        genome.push_back(g);
    }

    const std::vector<Gene>& Genome::genes() const
    {
        return genome;
    }

    bool Genome::empty() const
    {
        return genome.empty();
    }

    size_t Genome::size() const 
    { 
        return genome.size();
    }

    /**
     * remove length elements from the start
    */
    void Genome::erase(unsigned length)
    {
        unsigned number = genome.size() - length;
        genome.erase(genome.begin(), genome.begin() + number);
    }

    /**
     * remove length elements from the end
    */
    void Genome::eraseBack(unsigned length)
    {
        genome.erase(genome.end() - (genome.size() - length), genome.end());
    }

    void Genome::eraseAt(unsigned index)
    {
        genome.erase(genome.begin() + index);
    }

    const Gene& Genome::geneAt(unsigned index) const
    {
        assert(index < genome.size());
        return genome[index];
    }

    Gene& Genome::geneAt(unsigned index)
    {
        assert(index < genome.size());
        return genome[index];
    }

    void Genome::overlayWithSliceOf(const Genome &other, uint16_t index0, uint16_t index1)
    {
        std::copy(other.genes().begin() + index0, other.genes().begin() + index1, genome.begin() + index0);
    }

} // namespace BS
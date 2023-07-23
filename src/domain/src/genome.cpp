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

    // The jaro_winkler_distance() function is adapted from the C version at
    // https://github.com/miguelvps/c/blob/master/jarowinkler.c
    // under a GNU license, ver. 3. This comparison function is useful if
    // the simulator allows genomes to change length, or if genes are allowed
    // to relocate to different offsets in the genome. I.e., this function is
    // tolerant of gaps, relocations, and genomes of unequal lengths.
    //
    float Genome::jaroWinklerDistance(const Genome &other) const
    {
        float dw;
        auto max = [](int a, int b) { return a > b ? a : b; };
        auto min = [](int a, int b) { return a < b ? a : b; };

        const auto &s = genome;
        const auto &a = other;

        int i, j, l;
        int m = 0, t = 0;
        int sl = s.size(); // strlen(s);
        int al = a.size(); // strlen(a);

        constexpr unsigned maxNumGenesToCompare = 20;
        sl = min(maxNumGenesToCompare, sl); // optimization: approximate for long genomes
        al = min(maxNumGenesToCompare, al);

        std::vector<int> sflags(sl, 0);
        std::vector<int> aflags(al, 0);
        int range = max(0, max(sl, al) / 2 - 1);

        if (!sl || !al)
            return 0.0;

        /* calculate matching characters */
        for (i = 0; i < al; i++) {
            for (j = max(i - range, 0), l = min(i + range + 1, sl); j < l; j++) {
                
                if ((a.geneAt(i) == geneAt(j)) && !sflags[j]) {
                    sflags[j] = 1;
                    aflags[i] = 1;
                    m++;
                    break;
                }
            }
        }

        if (!m)
            return 0.0;

        /* calculate character transpositions */
        l = 0;
        for (i = 0; i < al; i++) {
            if (aflags[i] == 1) {
                for (j = l; j < sl; j++) {
                    if (sflags[j] == 1) {
                        l = j + 1;
                        break;
                    }
                }
                if (a.geneAt(i) != geneAt(j))
                    t++;
            }
        }
        t /= 2;

        /* Jaro distance */
        dw = (((float)m / sl) + ((float)m / al) + ((float)(m - t) / m)) / 3.0f;
        return dw;
    }

    // Works only for genomes of equal length
    float Genome::hammingDistanceBytes(const Genome &other) const
    {
        assert(genome.size() == other.size());

        std::vector<Gene> otherGenome = other.genes();

        const unsigned int *p1 = (const unsigned int *)genome.data();
        const unsigned int *p2 = (const unsigned int *)otherGenome.data();

        const unsigned numElements = genome.size();
        const unsigned bytesPerElement = sizeof(genome[0]);
        const unsigned lengthBytes = numElements * bytesPerElement;
        unsigned byteCount = 0;

        for (unsigned index = 0; index < genome.size(); ++p1, ++p2, ++index) {
            byteCount += (unsigned)(*p1 == *p2);
        }

        return byteCount / (float)lengthBytes;
    }

    // Works only for genomes of equal length
    float Genome::hammingDistanceBits(const Genome &other) const
    {
        assert(genome.size() == other.size());

        std::vector<Gene> otherGenome = other.genes();

        const unsigned int *p1 = (const unsigned int *)genome.data();
        const unsigned int *p2 = (const unsigned int *)otherGenome.data();
        const unsigned numElements = genome.size();
        const unsigned bytesPerElement = sizeof(genome[0]);
        const unsigned lengthBytes = numElements * bytesPerElement;
        const unsigned lengthBits = lengthBytes * 8;
        unsigned bitCount = 0;

        for (unsigned index = 0; index < genome.size(); ++p1, ++p2, ++index) {
            bitCount += __builtin_popcount(*p1 ^ *p2);
        }

        // For two completely random bit patterns, about half the bits will differ,
        // resulting in c. 50% match. We will scale that by 2X to make the range
        // from 0 to 1.0. We clip the value to 1.0 in case the two patterns are
        // negatively correlated for some reason.
        return 1.0 - std::min(1.0, (2.0 * bitCount) / (float)lengthBits);
    }

} // namespace BS
// genome-compare.cpp -- compute similarity of two genomes

#include <cassert>
#include "simulator.h"
#include "domain/include/genome.h"

namespace BS {

// Returns 0.0..1.0
//
// ToDo: optimize by approximation for long genomes
float genomeSimilarity(const std::shared_ptr<Genome> g1, const std::shared_ptr<Genome> g2)
{
    switch (p.genomeComparisonMethod) {
    case 0:
        return g1->jaroWinklerDistance(g2);
    case 1:
        return g1->hammingDistanceBits(g2);
    case 2:
        return g1->hammingDistanceBytes(g2);
    default:
        assert(false);
    }
}

// returns 0.0..1.0
// Samples random pairs of individuals regardless if they are alive or not
float geneticDiversity()
{
    if (p.population < 2) {
        return 0.0;
    }

    // count limits the number of genomes sampled for performance reasons.
    unsigned count = std::min(1000U, p.population);    // todo: !!! p.analysisSampleSize;
    int numSamples = 0;
    float similaritySum = 0.0f;

    while (count > 0) {
        unsigned index0 = randomUint(1, p.population - 1); // skip first and last elements
        unsigned index1 = index0 + 1;
        similaritySum += genomeSimilarity(peeps[index0].genome, peeps[index1].genome);
        --count;
        ++numSamples;
    }
    float diversity = 1.0f - (similaritySum / numSamples);
    return diversity;
}

} // end namespace BS

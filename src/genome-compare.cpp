// genome-compare.cpp -- compute similarity of two genomes

#include <cassert>
#include "simulator.h"

namespace BS {

// Approximate gene match: Has to match same source, sink, with similar weight
//
bool genesMatch(const Gene &g1, const Gene &g2)
{
    return g1.sinkNum == g2.sinkNum
        && g1.sourceNum == g2.sourceNum
        && g1.sinkType == g2.sinkType
        && g1.sourceType == g2.sourceType
        && g1.weight == g2.weight;
}


// The jaro_winkler_distance() function is adapted from the C version at
// https://github.com/miguelvps/c/blob/master/jarowinkler.c
// under a GNU license, ver. 3. This comparison function is useful if
// the simulator allows genomes to change length, or if genes are allowed
// to relocate to different offsets in the genome. I.e., this function is
// tolerant of gaps, relocations, and genomes of unequal lengths.
//
float jaro_winkler_distance(const Genome &genome1, const Genome &genome2) {
    float dw;
    auto max = [](int a, int b) { return a > b ? a : b; };
    auto min = [](int a, int b) { return a < b ? a : b; };

    const auto &s = genome1;
    const auto &a = genome2;

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
            if (genesMatch(a[i], s[j]) && !sflags[j]) {
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
            if (!genesMatch(a[i], s[j]))
                t++;
        }
    }
    t /= 2;

    /* Jaro distance */
    dw = (((float)m / sl) + ((float)m / al) + ((float)(m - t) / m)) / 3.0f;
    return dw;
}


// Works only for genomes of equal length
float hammingDistanceBits(const Genome &genome1, const Genome &genome2)
{
    assert(genome1.size() == genome2.size());

    const unsigned int *p1 = (const unsigned int *)genome1.data();
    const unsigned int *p2 = (const unsigned int *)genome2.data();
    const unsigned numElements = genome1.size();
    const unsigned bytesPerElement = sizeof(genome1[0]);
    const unsigned lengthBytes = numElements * bytesPerElement;
    const unsigned lengthBits = lengthBytes * 8;
    unsigned bitCount = 0;

    for (unsigned index = 0; index < genome1.size(); ++p1, ++p2, ++index) {
        bitCount += __builtin_popcount(*p1 ^ *p2);
    }

    // For two completely random bit patterns, about half the bits will differ,
    // resulting in c. 50% match. We will scale that by 2X to make the range
    // from 0 to 1.0. We clip the value to 1.0 in case the two patterns are
    // negatively correlated for some reason.
    return 1.0 - std::min(1.0, (2.0 * bitCount) / (float)lengthBits);
}


// Works only for genomes of equal length
float hammingDistanceBytes(const Genome &genome1, const Genome &genome2)
{
    assert(genome1.size() == genome2.size());

    const unsigned int *p1 = (const unsigned int *)genome1.data();
    const unsigned int *p2 = (const unsigned int *)genome2.data();
    const unsigned numElements = genome1.size();
    const unsigned bytesPerElement = sizeof(genome1[0]);
    const unsigned lengthBytes = numElements * bytesPerElement;
    unsigned byteCount = 0;

    for (unsigned index = 0; index < genome1.size(); ++p1, ++p2, ++index) {
        byteCount += (unsigned)(*p1 == *p2);
    }

    return byteCount / (float)lengthBytes;
}


// Returns 0.0..1.0
//
// ToDo: optimize by approximation for long genomes
float genomeSimilarity(const Genome &g1, const Genome &g2)
{
    switch (p.genomeComparisonMethod) {
    case 0:
        return jaro_winkler_distance(g1, g2);
    case 1:
        return hammingDistanceBits(g1, g2);
    case 2:
        return hammingDistanceBytes(g1, g2);
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

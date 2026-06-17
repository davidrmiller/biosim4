// genome-compare.cpp -- compute similarity of two genomes

#include <cassert>
#include <algorithm>
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

    const auto &s = genome1;
    const auto &a = genome2;

    int i, j, l;
    int m = 0, t = 0;
    int sl = s.size(); // strlen(s);
    int al = a.size(); // strlen(a);

    constexpr unsigned maxNumGenesToCompare = 20;
    sl = std::min((int)maxNumGenesToCompare, sl); // optimization: approximate for long genomes
    al = std::min((int)maxNumGenesToCompare, al);

    std::vector<int> sflags(sl, 0);
    std::vector<int> aflags(al, 0);
    int range = std::max(0, std::max(sl, al) / 2 - 1);

    if (!sl || !al)
        return 0.0;

    /* calculate matching characters */
    for (i = 0; i < al; i++) {
        for (j = std::max(i - range, 0), l = std::min(i + range + 1, sl); j < l; j++) {
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
            if (j < sl && !genesMatch(a[i], s[j]))  // Fixed: check bounds
                t++;
        }
    }
    t /= 2;

    /* Jaro distance */
    dw = (((float)m / sl) + ((float)m / al) + ((float)(m - t) / m)) / 3.0f;

    // Winkler prefix bonus: boost similarity if genomes start similarly
    constexpr int maxPrefixLength = 4;
    int prefixLength = std::min(maxPrefixLength, std::min(sl, al));
    int matchingPrefix = 0;
    for (int i = 0; i < prefixLength; i++) {
        if (genesMatch(s[i], a[i])) {
            matchingPrefix++;
        } else {
            break;
        }
    }

    // Winkler scaling factor (typically 0.1)
    constexpr float winklerScaling = 0.1f;
    float winklerBonus = winklerScaling * matchingPrefix * (1.0f - dw);

    return std::min(1.0f, dw + winklerBonus);
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
    float similarity;
    
    // If genomes have different lengths, use Jaro-Winkler (method 0) which handles unequal lengths
    if (g1.size() != g2.size()) {
        similarity = jaro_winkler_distance(g1, g2);
        
        // Add length penalty to prevent convergence to extreme lengths
        // Penalize based on relative length difference
        float len1 = (float)g1.size();
        float len2 = (float)g2.size();
        float lengthRatio = std::min(len1, len2) / std::max(len1, len2);
        
        // Add absolute length penalty: penalize genomes that deviate from initial length
        // This creates selection pressure to maintain lengths near the starting value
        float initialLength = (float)p.genomeInitialLengthMin;
        float avgLength = (len1 + len2) / 2.0f;
        float lengthDeviation = std::abs(avgLength - initialLength) / initialLength;
        // Penalty increases quadratically with deviation (0.0 at initial, 1.0 at 2x initial)
        float absolutePenalty = std::min(lengthDeviation / 2.0f, 1.0f);
        float absoluteBonus = 1.0f - absolutePenalty;
        
        // Apply penalties: 30% similarity, 35% relative length ratio, 35% absolute length bonus
        // Strengthened length penalties to better prevent genome length growth
        // This triple penalty system prevents both relative divergence and absolute growth
        similarity = similarity * 0.3f + lengthRatio * 0.35f + absoluteBonus * 0.35f;
        
        return similarity;
    }
    
    switch (p.genomeComparisonMethod) {
    case 0:
        similarity = jaro_winkler_distance(g1, g2);
        break;
    case 1:
        similarity = hammingDistanceBits(g1, g2);
        break;
    case 2:
        similarity = hammingDistanceBytes(g1, g2);
        break;
    default:
        assert(false);
        similarity = 0.0f;
    }
    
    return similarity;
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

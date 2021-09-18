// random.cpp
// Provides a random number generator for the main thread
// and child threads

#include <cassert>
#include <cmath>
#include <random>
#include <chrono>
#include <climits>
#include "random.h"
#include "omp.h"

namespace BS {


// Default is determinstic
RandomUintGenerator::RandomUintGenerator(bool deterministic)
{
    if (deterministic) {
        // for Marsaglia
        rngx = 123456789;
        rngy = 362436000;
        rngz = 521288629;
        rngc = 7654321;

        // for Jenkins:
        a = 0xf1ea5eed, b = c = d = 123456789;
    } else {
        randomize();
    }
}


void RandomUintGenerator::randomize()
{
    std::mt19937 generator(time(0));  // mt19937 is a standard mersenne_twister_engine

    // for Marsaglia
    do { rngx = generator(); } while (rngx == 0);
    do { rngy = generator(); } while (rngy == 0);
    do { rngz = generator(); } while (rngz == 0);
    do { rngc = generator(); } while (rngc == 0);

    // for Jenkins:
    a = 0xf1ea5eed, b = c = d = generator();
}


// This algorithm is from http://www0.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf
// where it is attributed to G. Marsaglia.
//
uint32_t RandomUintGenerator::operator()()
{
    if (false) {
        // Marsaglia
        uint64_t t, a = 698769069ULL;
        rngx = 69069 * rngx + 12345;
        rngy ^= (rngy << 13);
        rngy ^= (rngy >> 17);
        rngy ^= (rngy << 5); /* y must never be set to zero! */
        t = a * rngz + rngc;
        rngc = (t >> 32);/* Also avoid setting z=c=0! */
        return rngx + rngy + (rngz = t);
    } else {
        // Jenkins
        #define rot32(x,k) (((x)<<(k))|((x)>>(32-(k))))
        uint32_t e = a - rot32(b, 27);
        a = b ^ rot32(c, 17);
        b = c + d;
        c = d + e;
        d = e + a;
        return d;
    }
}


// Sure, there's a bias when using modulus operator where (max - min) is not
// a power of two, but we don't care if we generate one value a little more
// often than another. Our randomness does not have to be any better quality
// than the randomness of a shotgun. We do care about speed, because this will
// get called inside deeply nested inner loops.
//
unsigned RandomUintGenerator::operator()(unsigned min, unsigned max)
{
    assert(max >= min);
    return ((*this)() % (max - min + 1)) + min;
}


// The globally accessible random number generator. Threads can be
// given their own private copies of this.
RandomUintGenerator randomUint;

} // end namespace BS

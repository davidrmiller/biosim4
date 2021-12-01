// random.cpp

// This file provides a random number generator (RNG) for the main thread
// and child threads. There is a global RNG instance named randomUint which
// can be duplicated so that each thread gets a private copy. The global
// RNG must be initialized after program startup by calling its initialize()
// member function, typically at the top of simulator() in simulator.cpp
// after the parameters have been read. The parameters named "deterministic"
// and "RNGSeed" determine whether to initialize the RNG with a
// user-defined deterministic seed or with a random seed.

#include <cassert>
#include <cmath>
#include <random>
#include <chrono>
#include <climits>
#include "simulator.h"


namespace BS {


RandomUintGenerator::RandomUintGenerator()
    : initialized(false)
{
}


// If parameter p.deterministic is true, we'll initialize the RNG with
// the seed specified in parameter p.RNGSeed, otherwise we'll initialize
// the RNG with a random seed. This initializes both the Marsaglia and
// the Jenkins algorithms. The member function operator() determines
// which algorithm is actually used.
void RandomUintGenerator::initialize()
{
    if (p.deterministic) {
        // Initialize Marsaglia. Overflow wrap-around is ok. We just want
        // the four parameters to be unrelated:
        rngx = p.RNGSeed + 123456789;
        rngy = p.RNGSeed + 362436000;
        rngz = p.RNGSeed + 521288629;
        rngc = p.RNGSeed + 7654321;

        // Initialize Jenkins:
        a = 0xf1ea5eed;
        b = c = d = p.RNGSeed;
    } else {
        // Non-deterministic
        // Get a random seed from the built-in generator
        std::mt19937 generator(time(0));  // mt19937 is a standard mersenne_twister_engine

        // Initialize Marsaglia, but don't let any of the values be zero:
        do { rngx = generator(); } while (rngx == 0);
        do { rngy = generator(); } while (rngy == 0);
        do { rngz = generator(); } while (rngz == 0);
        do { rngc = generator(); } while (rngc == 0);

        // Initialize Jenkins, but don't let any of the values be zero:
        a = 0xf1ea5eed;
        do { b = c = d = generator(); } while (b == 0);
    }

    initialized = true; // for debugging, remember that we initialized the RNG
}


// This returns a random 32-bit integer. Neither the Marsaglia nor the Jenkins
// algorithms are of cryptographic quality, but we don't need that. We just need
// randomness of shotgun quality. The Jenkins algorithm is the fastest.
// The Marsaglia algorithm is from http://www0.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf
// where it is attributed to G. Marsaglia.
//
uint32_t RandomUintGenerator::operator()()
{
    if (false) {
        // Marsaglia algorithm
        uint64_t t, a = 698769069ULL;
        rngx = 69069 * rngx + 12345;
        rngy ^= (rngy << 13);
        rngy ^= (rngy >> 17);
        rngy ^= (rngy << 5); /* y must never be set to zero! */
        t = a * rngz + rngc;
        rngc = (t >> 32);/* Also avoid setting z=c=0! */
        return rngx + rngy + (rngz = t);
    } else {
        // Jenkins algorithm
        #define rot32(x,k) (((x)<<(k))|((x)>>(32-(k))))
        uint32_t e = a - rot32(b, 27);
        a = b ^ rot32(c, 17);
        b = c + d;
        c = d + e;
        d = e + a;
        return d;
    }
}


// Returns an unsigned integer between min and max, inclusive.
// Sure, there's a bias when using modulus operator where (max - min) is not
// a power of two, but we don't care if we generate one value a little more
// often than another. Our randomness does not have to be high quality.
// We do care about speed, because this will get called inside deeply nested
// inner loops.
//
unsigned RandomUintGenerator::operator()(unsigned min, unsigned max)
{
    assert(max >= min);
    return ((*this)() % (max - min + 1)) + min;
}


// This is the globally-accessible random number generator. Threads can
// be given their own private copies of this. This object needs to be
// initialized once before its first use by calling randomUint.initialize().
RandomUintGenerator randomUint;

} // end namespace BS

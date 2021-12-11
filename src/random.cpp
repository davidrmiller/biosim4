// random.cpp

// This file provides a random number generator (RNG) for the main thread
// and child threads. The global-scoped RNG instance named randomUint is declared
// "threadprivate" for OpenMP, meaning that each thread will instantiate its
// own private instance. A side effect is that the object cannot have a
// non-trivial ctor, so it has an initialize() member function that must be
// called to seed the RNG instance, typically in simulator() in simulator.cpp
// after the config parameters have been read. The biosim4.ini parameters named
// "deterministic" and "RNGSeed" determine whether to initialize the RNG with
// a user-defined deterministic seed or with a random seed.

#include <cassert>
#include <cmath>
#include <random>
#include <chrono>
#include <climits>
#include "simulator.h"
#include "omp.h"


namespace BS {


// If parameter p.deterministic is true, we'll initialize the RNG with
// the seed specified in parameter p.RNGSeed, otherwise we'll initialize
// the RNG with a random seed. This initializes both the Marsaglia and
// the Jenkins algorithms. The member function operator() determines
// which algorithm is actually used.
void RandomUintGenerator::initialize()
{
    if (p.deterministic) {
        // Initialize Marsaglia. Overflow wrap-around is ok. We just want
        // the four parameters to be unrelated. In the extremely unlikely
        // event that a coefficient is zero, we'll force it to an arbitrary
        // non-zero value. Each thread uses a different seed, yet
        // deterministic per-thread.
        rngx = p.RNGSeed + 123456789 + omp_get_thread_num();
        rngy = p.RNGSeed + 362436000 + omp_get_thread_num();
        rngz = p.RNGSeed + 521288629 + omp_get_thread_num();
        rngc = p.RNGSeed + 7654321 + omp_get_thread_num();
        rngx = rngx != 0 ? rngx : 123456789;
        rngy = rngy != 0 ? rngy : 123456789;
        rngz = rngz != 0 ? rngz : 123456789;
        rngc = rngc != 0 ? rngc : 123456789;

        // Initialize Jenkins determinstically per-thread:
        a = 0xf1ea5eed;
        b = c = d = p.RNGSeed + omp_get_thread_num();
        if (b == 0) {
            b = c = d + 123456789;
        }
    } else {
        // Non-deterministic initialization.
        // First we will get a random number from the built-in mt19937
        // (Mersenne twister) generator and use that to derive the
        // starting coefficients for the Marsaglia and Jenkins RNGs.
        // We'll seed mt19937 with time(), but that has a coarse
        // resolution and multiple threads might be initializing their
        // instances at nearly the same time, so we'll add the thread
        // number to uniquely seed mt19937 per-thread.
        std::mt19937 generator(time(0) + omp_get_thread_num());

        // Initialize Marsaglia, but don't let any of the values be zero:
        do { rngx = generator(); } while (rngx == 0);
        do { rngy = generator(); } while (rngy == 0);
        do { rngz = generator(); } while (rngz == 0);
        do { rngc = generator(); } while (rngc == 0);

        // Initialize Jenkins, but don't let any of the values be zero:
        a = 0xf1ea5eed;
        do { b = c = d = generator(); } while (b == 0);
    }
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
// inner loops. Alternatively, we could create a standard C++ "distribution"
// object here, but we would first need to investigate its overhead.
//
unsigned RandomUintGenerator::operator()(unsigned min, unsigned max)
{
    assert(max >= min);
    return ((*this)() % (max - min + 1)) + min;
}


// This is the globally-accessible random number generator. Declaring
// it threadprivate causes each thread to instantiate a private instance.
RandomUintGenerator randomUint;
#pragma omp threadprivate(randomUint)

} // end namespace BS

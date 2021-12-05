#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

// See random.cpp for notes.

#include <cstdint>
#include <climits>

namespace BS {


struct RandomUintGenerator{
private:
    // for the Marsaglia algorithm
    uint32_t rngx;
    uint32_t rngy;
    uint32_t rngz;
    uint32_t rngc;
    // for the Jenkins algorithm
    uint32_t a, b, c, d;
public:
    void initialize(); // must be called to seed the RNG
    uint32_t operator()();
    unsigned operator()(unsigned min, unsigned max);
};

// The globally-scoped random number generator. Declaring it
// threadprivate causes each thread to instantiate a private instance.
extern RandomUintGenerator randomUint;
#pragma omp threadprivate(randomUint)

constexpr uint32_t RANDOM_UINT_MAX = 0xffffffff;

} // end namespace BS

#endif // RANDOM_H_INCLUDED

#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

// See random.cpp for notes.

#include <cstdint>
#include <climits>

namespace BS {

extern int randomInt(int min = 0, int max = INT_MAX);
extern uint16_t randomU16(uint16_t min = 0, uint16_t max = (uint16_t)-1);
extern float randomFloat01(); // 0.0..1.0
extern float randomFloat11(); // -1.0..1.0

class RandomUintGenerator{
private:
    // for the Marsaglia algorithm
    uint32_t rngx;
    uint32_t rngy;
    uint32_t rngz;
    uint32_t rngc;
    // for the Jenkins algorithm
    uint32_t a, b, c, d;
    // for debugging, record whether the RNG got initialized properly
    bool initialized;
public:
    RandomUintGenerator();
    RandomUintGenerator& operator=(const RandomUintGenerator &rhs) = default;
    void initialize();
    uint32_t operator()();
    unsigned operator()(unsigned min, unsigned max);
};

// The globally accessible random number generator (not thread safe -- for
// multi-threaded use, each thread must be given a private copy of this object)
extern RandomUintGenerator randomUint;
constexpr uint32_t RANDOM_UINT_MAX = 0xffffffff;

} // end namespace BS

#endif // RANDOM_H_INCLUDED

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
    // for Marsaglia
    uint32_t rngx;
    uint32_t rngy;
    uint32_t rngz;
    uint32_t rngc;
    // for Jenkins
    uint32_t a, b, c, d;
public:
    RandomUintGenerator(bool deterministic = false);
    RandomUintGenerator& operator=(const RandomUintGenerator &rhs) = default;
    void randomize();
    uint32_t operator()();
    unsigned operator()(unsigned min, unsigned max);
};

// The globally accessible random number generator (not thread safe)
extern RandomUintGenerator randomUint;
constexpr uint32_t RANDOM_UINT_MAX = 0xffffffff;

} // end namespace BS

#endif // RANDOM_H_INCLUDED

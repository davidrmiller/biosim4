#include "../include/polar.h"
// #include "dir.h"
#include "../include/coord.h"

namespace BS
{
    
    const Coord NormalizedCoords[9] = { 
        Coord(-1,-1), // SW
        Coord(0,-1),  // S
        Coord(1,-1),  // SE
        Coord(-1,0),  // W
        Coord(0,0),   // CENTER
        Coord(1,0),   // E
        Coord(-1,1),  // NW
        Coord(0,1),   // N
        Coord(1,1)    // NE
    };

    Polar::Polar(int mag0, Compass dir0)
            : mag{mag0}, dir{Dir{dir0}} { }
            
    Polar::Polar(int mag0, Dir dir0)
            : mag{mag0}, dir{dir0} { }

    /*
        Compass values:

            6  7  8
            3  4  5
            0  1  2
    */
    Coord Polar::asCoord() const
    {
        // (Thanks to @Asa-Hopkins for this optimized function -- drm)

        // 3037000500 is 1/sqrt(2) in 32.32 fixed point
        constexpr int64_t coordMags[9] = {
            3037000500,  // SW
            1LL << 32,   // S
            3037000500,  // SE
            1LL << 32,   // W
            0,           // CENTER
            1LL << 32,   // E
            3037000500,  // NW
            1LL << 32,   // N
            3037000500   // NE
        };

        int64_t len = coordMags[dir.asInt()] * mag;

        // We need correct rounding, the idea here is to add/sub 1/2 (in fixed point)
        // and truncate. We extend the sign of the magnitude with a cast,
        // then shift those bits into the lower half, giving 0 for mag >= 0 and
        // -1 for mag<0. An XOR with this copies the sign onto 1/2, to be exact
        // we'd then also subtract it, but we don't need to be that precise.

        int64_t temp = ((int64_t)mag >> 32) ^ ((1LL << 31) - 1);
        len = (len + temp) / (1LL << 32); // Divide to make sure we get an arithmetic shift

        return NormalizedCoords[dir.asInt()] * len;
    }

} // namespace BS

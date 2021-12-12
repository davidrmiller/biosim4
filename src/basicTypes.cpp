// basicTypes.cpp

#include <cassert>
#include "basicTypes.h"

namespace BS {

// This rotates a Dir value by the specified number of steps. There are
// eight steps per full rotation. Positive values are clockwise; negative
// values are counterclockwise. E.g., rotate(4) returns a direction 90
// degrees to the right.
constexpr Compass NW = BS::Compass::NW; constexpr Compass N = BS::Compass::N;
constexpr Compass NE = BS::Compass::NE; constexpr Compass E = BS::Compass::E;
constexpr Compass SE = BS::Compass::SE; constexpr Compass S = BS::Compass::S;
constexpr Compass SW = BS::Compass::SW; constexpr Compass W = BS::Compass::W;
constexpr Compass C = BS::Compass::CENTER;

const Dir rotations[72] = { SW, W, NW, N, NE, E, SE, S,
                            S, SW, W, NW, N, NE, E, SE,
                            SE, S, SW, W, NW, N, NE, E,
                            W, NW, N, NE, E, SE, S, SW,
                            C, C, C, C, C, C, C, C,
                            E, SE, S, SW, W, NW, N, NE,
                            NW, N, NE, E, SE, S, SW, W,
                            N, NE, E, SE, S, SW, W, NW,
                            NE, E, SE, S, SW, W, NW, N };

Dir Dir::rotate(int n) const
{
    return rotations[asInt() * 8 + (n & 7)];
}


/*
    A normalized Coord is a Coord with x and y == -1, 0, or 1.
    A normalized Coord may be used as an offset to one of the
    8-neighbors.

    A Dir value maps to a normalized Coord using

       Coord { (d%3) - 1, (trunc)(d/3) - 1  }

       0 => -1, -1  SW
       1 =>  0, -1  S
       2 =>  1, -1, SE
       3 => -1,  0  W
       4 =>  0,  0  CENTER
       5 =>  1   0  E
       6 => -1,  1  NW
       7 =>  0,  1  N
       8 =>  1,  1  NE
*/
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


Coord Dir::asNormalizedCoord() const
{
    return NormalizedCoords[asInt()];
}


Polar Dir::asNormalizedPolar() const
{
    return Polar{1, dir9};
}


/*
    A normalized Coord has x and y == -1, 0, or 1.
    A normalized Coord may be used as an offset to one of the
    8-neighbors.
    We'll convert the Coord into a Dir, then convert Dir to normalized Coord.
*/
Coord Coord::normalize() const
{
    return asDir().asNormalizedCoord();
}


// Effectively, we want to check if a coordinate lies in a 45 degree region (22.5 degrees each side)
// centered on each compass direction. By first rotating the system by 22.5 degrees clockwise
// the boundaries to these regions become much easier to work with as they just align with the 8 axes.
// (Thanks to @Asa-Hopkins for this optimization -- drm)
Dir Coord::asDir() const
{
    // tanN/tanD is the best rational approximation to tan(22.5) under the constraint that
    // tanN + tanD < 2**16 (to avoid overflows). We don't care about the scale of the result,
    // only the ratio of the terms. The actual rotation is (22.5 - 1.5e-8) degrees, whilst
    // the closest a pair of int16_t's come to any of these lines is 8e-8 degrees, so the result is exact
    constexpr uint16_t tanN = 13860;
    constexpr uint16_t tanD = 33461;
    const Dir conversion[16] {  S, C, SW, N, SE, E, N,
                                N, N, N, W, NW, N, NE, N, N};

    const int32_t xp = x * tanD + y * tanN;
    const int32_t yp = y * tanD - x * tanN;

    // We can easily check which side of the four boundary lines
    // the point now falls on, giving 16 cases, though only 9 are
    // possible.
    return conversion[(yp > 0) * 8 + (xp > 0) * 4 + (yp > xp) * 2 + (yp >= -xp)];
}


Polar Coord::asPolar() const
{
    return Polar{(int)length(), asDir()};
}


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
        1L << 32,    // S
        3037000500,  // SE
        1L << 32,    // W
        0,           // CENTER
        1L << 32,    // E
        3037000500,  // NW
        1L << 32,    // N
        3037000500   // NE
    };

    int64_t len = coordMags[dir.asInt()] * mag;

    // We need correct rounding, the idea here is to add/sub 1/2 (in fixed point)
    // and truncate. We extend the sign of the magnitude with a cast,
    // then shift those bits into the lower half, giving 0 for mag >= 0 and
    // -1 for mag<0. An XOR with this copies the sign onto 1/2, to be exact
    // we'd then also subtract it, but we don't need to be that precise.

    int64_t temp = ((int64_t)mag >> 32) ^ ((1L << 31) - 1);
    len = (len + temp) / (1L << 32); // Divide to make sure we get an arithmetic shift

    return NormalizedCoords[dir.asInt()] * len;
}


// returns -1.0 (opposite directions) .. +1.0 (same direction)
// returns 1.0 if either vector is (0,0)
float Coord::raySameness(Coord other) const
{
    int64_t mag = ((int64_t)x * x + y * y) * (other.x * other.x + other.y * other.y);
    if (mag == 0) {
        return 1.0; // anything is "same" as zero vector
    }

    return (x * other.x + y * other.y) / std::sqrt(mag);
}


// returns -1.0 (opposite directions) .. +1.0 (same direction)
// returns 1.0 if self is (0,0) or d is CENTER
float Coord::raySameness(Dir d) const
{
    return raySameness(d.asNormalizedCoord());
}

} // end namespace BS

#include "../include/dir.h"

#include "../include/polar.h"
#include "../include/coord.h"

namespace BS
{
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

    Dir::Dir()
    {

    }


    Dir::Dir(Compass dir = Compass::CENTER) : dir9 {dir} {}

    Dir Dir::rotate(int n) const
    {
        return rotations[asInt() * 8 + (n & 7)];
    }

    Coord Dir::asNormalizedCoord() const
    {
        return NormalizedCoords[asInt()];
    }


    Polar Dir::asNormalizedPolar() const
    {
        return Polar{1, dir9};
    }

    Dir& Dir::operator=(const Compass& d) 
    { 
        dir9 = d; 
        return *this; 
    }

    uint8_t Dir::asInt() const 
    { 
        return (uint8_t)dir9; 
    }

        
    Dir Dir::rotate90DegCW() const 
    { 
        return rotate(2);
    }

    Dir Dir::rotate90DegCCW() const
    { 
        return rotate(-2); 
    }

    Dir Dir::rotate180Deg() const
    { 
        return rotate(4);
    }

    bool Dir::operator==(Compass d) const 
    { 
        return asInt() == (uint8_t)d;
    }

    bool Dir::operator!=(Compass d) const
    { 
        return asInt() != (uint8_t)d; 
    }

    bool Dir::operator==(Dir d) const
    { 
        return asInt() == d.asInt(); 
    }

    bool Dir::operator!=(Dir d) const
    { 
        return asInt() != d.asInt(); 
    }

} // namespace BS
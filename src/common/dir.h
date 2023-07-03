//#ifndef DIR_H_INCLUDED
//#define DIR_H_INCLUDED
#pragma once

#include "compass.h"
// #include "polar.h"
// #include "coord.h"
// #include "common.h"
// #include "../random.h"

namespace BS {

    struct Polar;
    struct Coord;

    /**
     * Supports the eight directions in enum class Compass plus CENTER.
    */
    struct __attribute__((packed)) Dir {

        Dir();
        
        Dir(Compass dir);// : dir9{dir} {}

        Dir& operator=(const Compass& d);// { dir9 = d; return *this; }
        uint8_t asInt() const;// { return (uint8_t)dir9; }
        Coord asNormalizedCoord() const;  // (-1, -0, 1, -1, 0, 1)
        Polar asNormalizedPolar() const;

        Dir rotate(int n = 0) const;
        Dir rotate90DegCW() const;// { return rotate(2); }
        Dir rotate90DegCCW() const;// { return rotate(-2); }
        Dir rotate180Deg() const;// { return rotate(4); }

        bool operator==(Compass d) const;// { return asInt() == (uint8_t)d; }
        bool operator!=(Compass d) const;// { return asInt() != (uint8_t)d; }
        bool operator==(Dir d) const;// { return asInt() == d.asInt(); }
        bool operator!=(Dir d) const;// { return asInt() != d.asInt(); }

    private:
        Compass dir9;
    };

} // NAMESPACE

// #endif // DIR_H_INCLUDED
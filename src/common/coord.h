// #ifndef COORD_H_INCLUDED
// #define COORD_H_INCLUDED
#pragma once

#include <cmath>
// #include <cstdint>

// #include "compass.h"
// #include <cmath>
// #include <cstdint>
// #include "dir.h"
// #include "polar.h"
// #include "common.h"

namespace BS {

    struct Dir;
    struct Polar;

    /**
     * Coordinates range anywhere in the range of int16_t. Coordinate arithmetic
     * wraps like int16_t. Can be used, e.g., for a location in the simulator grid, or
     * for the difference between two locations.
    */
    struct __attribute__((packed)) Coord {

        Coord();
        Coord(int16_t x0, int16_t y0);// : x{x0}, y{y0} { }
        bool isNormalized() const;// { return x >= -1 && x <= 1 && y >= -1 && y <= 1; }

        Coord normalize() const;
        unsigned length() const;// { return (int)(std::sqrt(x * x + y * y)); } // round down
        Dir asDir() const;
        Polar asPolar() const;

        bool operator==(Coord c) const;// { return x == c.x && y == c.y; }
        bool operator!=(Coord c) const;// { return x != c.x || y != c.y; }

        Coord operator+(Coord c) const;// { return Coord{(int16_t)(x + c.x), (int16_t)(y + c.y)}; }
        Coord operator-(Coord c) const;// { return Coord{(int16_t)(x - c.x), (int16_t)(y - c.y)}; }
        Coord operator*(int a) const;// { return Coord{(int16_t)(x * a), (int16_t)(y * a)}; }
        Coord operator+(Dir d) const;// { return *this + d.asNormalizedCoord(); }
        Coord operator-(Dir d) const;// { return *this - d.asNormalizedCoord(); }

        float raySameness(Coord other) const; // returns -1.0 (opposite) .. 1.0 (same)
        float raySameness(Dir d) const; // returns -1.0 (opposite) .. 1.0 (same)

    public:
        int16_t x;
        int16_t y;
    };

}

// #endif
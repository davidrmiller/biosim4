#pragma once

#include <cmath>

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
        Coord(int16_t x0, int16_t y0);
        bool isNormalized() const;

        Coord normalize() const;
        unsigned length() const;
        Dir asDir() const;
        Polar asPolar() const;

        bool operator==(Coord c) const;
        bool operator!=(Coord c) const;

        Coord operator+(Coord c) const;
        Coord operator-(Coord c) const;
        Coord operator*(int a) const;
        Coord operator+(Dir d) const;
        Coord operator-(Dir d) const;

        float raySameness(Coord other) const;
        float raySameness(Dir d) const;

    public:
        int16_t x;
        int16_t y;
    };

}

// #endif
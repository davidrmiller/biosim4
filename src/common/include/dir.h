#pragma once

#include "compass.h"

namespace BS {

    struct Polar;
    struct Coord;

    /**
     * Supports the eight directions in enum class Compass plus CENTER.
    */
    struct __attribute__((packed)) Dir {

        Dir();
        
        Dir(Compass dir);

        Dir& operator=(const Compass& d);
        uint8_t asInt() const;
        Coord asNormalizedCoord() const;  
        Polar asNormalizedPolar() const;

        Dir rotate(int n = 0) const;
        Dir rotate90DegCW() const;
        Dir rotate90DegCCW() const;
        Dir rotate180Deg() const;

        bool operator==(Compass d) const;
        bool operator!=(Compass d) const;
        bool operator==(Dir d) const;
        bool operator!=(Dir d) const;

    private:
        Compass dir9;
    };

}
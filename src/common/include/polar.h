// #ifndef POLAR_H_INCLUDED
// #define POLAR_H_INCLUDED
#pragma once

#include "compass.h"
#include "dir.h"

namespace BS {

    struct Dir;
    struct Coord;

    /**
     * Polar magnitudes are signed 32-bit integers so that they can extend across any 2D
     * area defined by the Coord class.
    */
    struct __attribute__((packed)) Polar {

        explicit Polar(int mag0 = 0, Compass dir0 = Compass::CENTER);
            
        explicit Polar(int mag0, Dir dir0);

        Coord asCoord() const;
        
    public:
        int mag;
        Dir dir;
    };

}

// #endif
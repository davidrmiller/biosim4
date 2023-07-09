// #ifndef COMPASS_H_INCLUDED
// #define COMPASS_H_INCLUDED
#pragma once
#include <cstdint>

namespace BS {

    enum class Compass :uint8_t 
    { 
        SW = 0, S, SE, W, CENTER, E, NW, N, NE 
    };

}

// #endif 
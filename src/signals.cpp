// signals.cpp
// Manages layers of pheremones

#include <cstdint>
#include "simulator.h"

namespace BS {

void Signals::init(uint16_t numLayers, uint16_t sizeX, uint16_t sizeY)
{
    data = std::vector<Layer>(numLayers, Layer(sizeX, sizeY));
}


// Increases the specified location by centerIncreaseAmount,
// and increases the neighboring cells by neighborIncreaseAmount

// Is it ok that multiple readers are reading this container while
// this single thread is writing to it?  todo!!!
void Signals::increment(uint16_t layerNum, Coord loc)
{
    constexpr float radius = 1.5;
    constexpr uint8_t centerIncreaseAmount = 2;
    constexpr uint8_t neighborIncreaseAmount = 1;

#pragma omp critical
    {
        visitNeighborhood(loc, radius, [layerNum](Coord loc) {
            if (signals[layerNum][loc.x][loc.y] < SIGNAL_MAX) {
                signals[layerNum][loc.x][loc.y] =
                        std::min<unsigned>(SIGNAL_MAX,
                                           signals[layerNum][loc.x][loc.y] + neighborIncreaseAmount);
            }
        });

        if (signals[layerNum][loc.x][loc.y] < SIGNAL_MAX) {
            signals[layerNum][loc.x][loc.y] =
                        std::min<unsigned>(SIGNAL_MAX,
                                           signals[layerNum][loc.x][loc.y] + centerIncreaseAmount);
        }
    }
}


// Fades the signals
void Signals::fade(unsigned layerNum)
{
    constexpr unsigned fadeAmount = 1;

    for (int16_t x = 0; x < p.sizeX; ++x) {
        for (int16_t y = 0; y < p.sizeY; ++y) {
            if (signals[layerNum][x][y] >= fadeAmount) {
                signals[layerNum][x][y] -= fadeAmount;  // fade center cell
            } else {
                signals[layerNum][x][y] = 0;
            }
        }
    }
}

} // end namespace BS

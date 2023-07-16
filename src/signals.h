#ifndef SIGNALS_H_INCLUDED
#define SIGNALS_H_INCLUDED

// Container for pheromones.

#include <vector>
#include <cstdint>
#include "common/include/column.h"
#include "common/include/layer.h"
namespace BS {

// Usage: uint8_t magnitude = signals[layer][x][y];
// or             magnitude = signals.getMagnitude(layer, Coord);


constexpr unsigned SIGNAL_MIN = 0;
constexpr unsigned SIGNAL_MAX = UINT8_MAX;

struct Signals {


    void init(uint16_t layers, uint16_t sizeX, uint16_t sizeY);
    Layer& operator[](uint16_t layerNum) { return data[layerNum]; }
    const Layer& operator[](uint16_t layerNum) const { return data[layerNum]; }
    uint8_t getMagnitude(uint16_t layerNum, Coord loc) const { return (*this)[layerNum][loc.x][loc.y]; }
    void increment(uint16_t layerNum, Coord loc);
    void zeroFill() { for (Layer &layer : data) { layer.zeroFill(); } }
    void fade(unsigned layerNum);
private:
    std::vector<Layer> data;
};

} // end namespace BS

#endif // SIGNALS_H_INCLUDED

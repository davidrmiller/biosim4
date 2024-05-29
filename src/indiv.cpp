// indiv.cpp

#include <iostream>
#include <cassert>
#include "simulator.h"

namespace BS {

// This is called when any individual is spawned.
// The responsiveness parameter will be initialized here to maximum value
// of 1.0, then depending on which action activation function is used,
// the default undriven value may be changed to 1.0 or action midrange.
void Indiv::initialize(uint16_t index_, Coord loc_, Genome &&genome_)
{
    index = index_;
    loc = loc_;
    //birthLoc = loc_;
    grid.set(loc_, index_);
    age = 0;
    oscPeriod = 34; // ToDo !!! define a constant
    alive = true;
    lastMoveDir = Dir::random8();
    responsiveness = 0.5; // range 0.0..1.0
    longProbeDist = p.longProbeDistance;
    challengeBits = (unsigned)false; // will be set true when some task gets accomplished
    genome = std::move(genome_);
    createWiringFromGenome();

    this->shape.setRadius(2);
    this->fillColor();
}

uint8_t Indiv::makeGeneticColor()
{
    return ((this->genome.size() & 1)
        | ((this->genome.front().sourceType)    << 1)
        | ((this->genome.back().sourceType)     << 2)
        | ((this->genome.front().sinkType)      << 3)
        | ((this->genome.back().sinkType)       << 4)
        | ((this->genome.front().sourceNum & 1) << 5)
        | ((this->genome.front().sinkNum & 1)   << 6)
        | ((this->genome.back().sourceNum & 1)  << 7));
}

void Indiv::fillColor()
{
    uint8_t rawColor = this->makeGeneticColor();
    uint8_t color[3];

    constexpr uint8_t minColorVal = 100;
    constexpr uint8_t minLumaVal = 50;
    auto rgbToLuma = [](uint8_t r, uint8_t g, uint8_t b) { return (r+r+r+b+g+g+g+g) / 8; };

    color[0] = (rawColor);                  // R: 0..255
    color[1] = ((rawColor & 0x1f) << 3);    // G: 0..255
    color[2] = ((rawColor & 7)    << 5);    // B: 0..255

    // Prevent color mappings to very bright colors (hard to see):
    if (rgbToLuma(color[0], color[1], color[2]) < minLumaVal) {
        if (color[0] < minColorVal) color[0] = 255 - color[0];
        if (color[1] < minColorVal) color[1] = 255 - color[1];
        if (color[2] < minColorVal) color[2] = 255 - color[2];
    }

	this->shape.setFillColor(sf::Color(color[0], color[1], color[2], 255));
}

} // end namespace BS

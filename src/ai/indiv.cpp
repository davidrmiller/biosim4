// indiv.cpp

#include <iostream>
#include <cassert>
#include <string.h>
#include <iomanip>
#include <cstdint>
#include <cstring>

#include "../simulator.h"
#include "indiv.h"
#include "../grid.h"

namespace BS {

extern std::string sensorShortName(Sensor sensor);
extern std::string actionShortName(Action action);
extern Grid grid;

// This is called when any individual is spawned.
// The responsiveness parameter will be initialized here to maximum value
// of 1.0, then depending on which action activation function is used,
// the default undriven value may be changed to 1.0 or action midrange.
void Indiv::initialize(uint16_t index_, Coord loc_, Genome &&genome_)
{
    index = index_;
    loc = loc_;
    birthLoc = Coord(loc.x, loc.y);
    genome = std::move(genome_);
    this->initVariables();
}

void Indiv::initVariables()
{
    grid.set(this->loc, this->index);

    age = 0;
    oscPeriod = 34; // ToDo !!! define a constant
    alive = true;
    lastMoveDir = Dir::random8();
    responsiveness = 0.5; // range 0.0..1.0
    longProbeDist = p.longProbeDistance;
    challengeBits = (unsigned)false; // will be set true when some task gets accomplished    
    createWiringFromGenome();

    this->shape.setRadius(2);
    this->fillColor();
}

/**
 * Generates a genetic color based on the properties of the Indiv's genome.
 *
 * @return A uint8_t representing the genetic color.
 */
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
    color[1] = ((rawColor & 0x1f) << 3);    // G: 0..255    & 00011111 << 3
    color[2] = ((rawColor & 7)    << 5);    // B: 0..255    & 00000111 << 5

    // Prevent color mappings to very bright colors (hard to see):
    if (rgbToLuma(color[0], color[1], color[2]) < minLumaVal) {
        if (color[0] < minColorVal) color[0] = 255 - color[0];
        if (color[1] < minColorVal) color[1] = 255 - color[1];
        if (color[2] < minColorVal) color[2] = 255 - color[2];
    }

	this->shape.setFillColor(sf::Color(color[0], color[1], color[2], 255));
}

// This prints a neural net in a form that can be processed with
// graph-nnet.py to produce a graphic illustration of the net.
std::stringstream Indiv::printIGraphEdgeList() const
{
    std::stringstream ss;
    for (auto & conn : nnet.connections) {
        if (conn.sourceType == SENSOR) {
            ss << sensorShortName((Sensor)(conn.sourceNum));
        } else {
            ss << "N" << std::to_string(conn.sourceNum);
        }

        ss << " ";

        if (conn.sinkType == ACTION) {
            ss << actionShortName((Action)(conn.sinkNum));
        } else {
            ss << "N" << std::to_string(conn.sinkNum);
        }

        ss << " " << std::to_string(conn.weight) << std::endl;
    }    
    return ss;
}


// Format: 32-bit hex strings, one per gene
void Indiv::printGenome() const
{
    constexpr unsigned genesPerLine = 8;
    unsigned count = 0;
    for (Gene gene : genome) {
        if (count == genesPerLine) {
            std::cout << std::endl;
            count = 0;
        } else if (count != 0) {
            std::cout << " ";
        }

        assert(sizeof(Gene) == 4);
        uint32_t n;
        std::memcpy(&n, &gene, sizeof(n));
        std::cout << std::hex << std::setfill('0') << std::setw(8) << n;
        ++count;
    }
    std::cout << std::dec << std::endl;
}

} // end namespace BS

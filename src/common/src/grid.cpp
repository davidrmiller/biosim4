// grid.cpp
#include <cstdint>
#include <functional>
#include <cassert>
#include "../include/grid.h"
#include "../include/dir.h"

namespace BS {

    Grid::Grid() : data {Layer(0, 0)} {}

    /**
     * Allocates space for the 2D grid with layers
    */
    void Grid::init(uint16_t numLayers, uint16_t sizeX, uint16_t sizeY)
    {
        data = Layer(sizeX, sizeY);
        layers = std::vector<Layer>(numLayers, Layer(sizeX, sizeY));
    }

    /**
     * Empty the grid, all layers and locations
    */
    void Grid::zeroFill()
    { 
        data.zeroFill();

        for (Layer &layer : layers) { 
            layer.zeroFill();
        }

        barrierLocations.clear();
        barrierCenters.clear();
    }

    uint16_t Grid::sizeX() const 
    { 
        return data.size(); 
    }

    uint16_t Grid::sizeY() const 
    { 
        if (data.size() > 0) {
            return data[0].size();
        } 
        return 0;
    }

    bool Grid::isInBounds(int16_t x, uint16_t y) const
    {
        return x >= 0 && x < sizeX() && y >= 0 && y < sizeY(); 
    }

    bool Grid::isInBounds(Coord loc) const 
    { 
        return loc.x >= 0 && loc.x < sizeX() && loc.y >= 0 && loc.y < sizeY(); 
    }

    bool Grid::isEmptyAt(Coord loc) const
    {
        return at(loc) == Grid::EMPTY; 
    }

    bool Grid::isBarrierAt(Coord loc) const
    { 
        return at(loc) == Grid::BARRIER; 
    }

    bool Grid::isOccupiedAt(Coord loc) const
    {
        return at(loc) != Grid::EMPTY && at(loc) != Grid::BARRIER;
    }

    bool Grid::isBorder(Coord loc) const
    {
        return loc.x == 0 || loc.x == sizeX() - 1 || loc.y == 0 || loc.y == sizeY() - 1;
    }

    uint16_t Grid::at(Coord loc) const
    { 
        if (isInBounds(loc)) {
            return data[loc.x][loc.y];
        } else {
            return 0;
        }
    }

    uint16_t Grid::at(uint16_t x, uint16_t y) const
    { 
        if (isInBounds(x,y) ) {
            return data[x][y];
        } else {
            // is this the correct response?
            return 0;
        }
    }
    
    bool Grid::hasLayer(uint16_t layerNum) const
    {
        return layerNum < layers.size();
    }

    void Grid::incrementLayer(uint16_t layerNum, Coord loc)
    {
        if (hasLayer(layerNum) && isInBounds(loc)){

            // constexpr float radius = 1.5;
            // constexpr uint8_t centerIncreaseAmount = 2;
            // constexpr uint8_t neighborIncreaseAmount = 1;

            // #pragma omp critical
            // {
            //     visitNeighborhood(loc, radius, [layerNum](Coord loc) {
            //         if (signals[layerNum][loc.x][loc.y] < SIGNAL_MAX) {
            //             signals[layerNum][loc.x][loc.y] =
            //                     std::min<unsigned>(SIGNAL_MAX,
            //                                     signals[layerNum][loc.x][loc.y] + neighborIncreaseAmount);
            //         }
            //     });

            //     if (layers[layerNum][loc.x][loc.y] < SIGNAL_MAX) {
            //         layers[layerNum][loc.x][loc.y] =
            //                     std::min<unsigned>(SIGNAL_MAX,
            //                                     layers[layerNum][loc.x][loc.y] + centerIncreaseAmount);
            //     }
            // }
        }
    }
    
    /**
    */
    uint8_t Grid::getLayerMagnitude(uint16_t layerNum, Coord loc) const
    {   
        if (hasLayer(layerNum) && isInBounds(loc)){
            return layers[layerNum][loc.x][loc.y];
        } else {
            return 0;
        }
    }
    
    /**
     * Iterates over the whole area of a single layer, reducing the value of each cell
     * use a GridRectangle  
     * TODO: parameterize fadeAmount, as an instance property?
    */
    void Grid::fadeLayer(unsigned layerNum)
    {
        constexpr unsigned fadeAmount = 1;

        if (hasLayer(layerNum)) {
            for (int16_t x = 0; x < sizeX(); ++x) {
                for (int16_t y = 0; y < sizeY(); ++y) {
                    if (layers[layerNum][x][y] >= fadeAmount) {
                        layers[layerNum][x][y] -= fadeAmount;  // fade center cell
                    } else {
                        layers[layerNum][x][y] = 0;
                    }
                }
            }
        }
    }

    bool Grid::set(Coord loc, uint16_t val)
    { 
        if (isInBounds(loc)){
            data[loc.x][loc.y] = val;
            return true;
        } else {
            return false;
        }
    }

/**
     * TODO: if this overwrites a barrier then remove from barrierLocations
    */
    bool Grid::set(uint16_t x, uint16_t y, uint16_t val)
    {
        if (isInBounds(x,y)){
            data[x][y] = val;
            return true;
        }
        return false;
    }

    bool Grid::setBarrier(int16_t x, int16_t y)
    {
        if (set(x, y, Grid::BARRIER)) {
            barrierLocations.push_back( {x, y} );

            return true;
        }
        
        return false;
    }

    bool Grid::setBarrier(Coord loc)
    {
        if (set(loc, Grid::BARRIER)){
            barrierLocations.push_back( loc );
            return true;
        }

        return false;
    }

    /**
     * TODO: prevent setting this when the loc is occupied
    */
    bool Grid::addBarrierCenter(Coord loc)
    {
        if (isInBounds(loc)){
            barrierCenters.push_back(loc);
            return true;
        }
        return false;
    }

    const std::vector<Coord> &Grid::getBarrierLocations() const
    { 
        return barrierLocations;
    }

    const std::vector<Coord> &Grid::getBarrierCenters() const
    {
        return barrierCenters;
    }

    // Direct access:
    Column & Grid::operator[](uint16_t columnXNum)
    { 
        return data[columnXNum];
    }

    const Column & Grid::operator[](uint16_t columnXNum) const
    { 
        return data[columnXNum];
    }

    // Returns the number of locations to the next barrier in the
    // specified direction, not including loc. Ignores agents in the way.
    // If the distance to the border is less than the longProbeDist distance
    // and no barriers are found, returns longProbeDist.
    // Returns 0..longProbeDist.
    unsigned Grid::longProbeBarrierFwd(Coord loc, Dir dir, unsigned longProbeDist)
    {
        assert(longProbeDist > 0);
        unsigned count = 0;
        loc = loc + dir;
        unsigned numLocsToTest = longProbeDist;
        while (numLocsToTest > 0 && isInBounds(loc) && !isBarrierAt(loc)) {
            ++count;
            loc = loc + dir;
            --numLocsToTest;
        }
        if (numLocsToTest > 0 && !isInBounds(loc)) {
            return longProbeDist;
        } else {
            return count;
        }
    }

    // Converts the number of locations (not including loc) to the next barrier location
    // along opposite directions of the specified axis to the sensor range. If no barriers
    // are found, the result is sensor mid-range. Ignores agents in the path.
    float Grid::getShortProbeBarrierDistance(Coord loc0, Dir dir, unsigned probeDistance)
    {
        unsigned countFwd = 0;
        unsigned countRev = 0;
        Coord loc = loc0 + dir;
        unsigned numLocsToTest = probeDistance;

        // Scan positive direction
        while (numLocsToTest > 0 && isInBounds(loc) && !isBarrierAt(loc)) {
            ++countFwd;
            loc = loc + dir;
            --numLocsToTest;
        }
        if (numLocsToTest > 0 && !isInBounds(loc)) {
            countFwd = probeDistance;
        }

        // Scan negative direction
        numLocsToTest = probeDistance;
        loc = loc0 - dir;

        while (numLocsToTest > 0 && isInBounds(loc) && !isBarrierAt(loc)) {
            ++countRev;
            loc = loc - dir;
            --numLocsToTest;
        }

        if (numLocsToTest > 0 && !isInBounds(loc)) {
            countRev = probeDistance;
        }

        /**
         * TODO: does this belong in Grid? and if not where?
        */
        float sensorVal = ((countFwd - countRev) + probeDistance); // convert to 0..2*probeDistance
        sensorVal = (sensorVal / 2.0) / probeDistance; // convert to 0.0..1.0
        return sensorVal;
    }

    // Returns the number of locations to the next agent in the specified
    // direction, not including loc. If the probe encounters a boundary or a
    // barrier before reaching the longProbeDist distance, returns longProbeDist.
    // Returns 0..longProbeDist.
    unsigned Grid::longProbePopulationFwd(Coord loc, Dir dir, unsigned longProbeDist)
    {
        assert(longProbeDist > 0);
        unsigned count = 0;
        loc = loc + dir;
        unsigned numLocsToTest = longProbeDist;
        while (numLocsToTest > 0 && isInBounds(loc) && isEmptyAt(loc)) {
            ++count;
            loc = loc + dir;
            --numLocsToTest;
        }
        if (numLocsToTest > 0 && (!isInBounds(loc) || isBarrierAt(loc))) {
            return longProbeDist;
        } else {
            return count;
        }
    }


} // end namespace BS

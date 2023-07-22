#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

// The grid is the 2D arena where the agents live.

#include <cstdint>
#include <vector>
#include <functional>

#include "gridLocationVisitor.h"
#include "coord.h"
#include "column.h"
#include "layer.h"

namespace BS {

    // Grid is a somewhat dumb 2D container of unsigned 16-bit values.
    // Grid understands that the elements are either EMPTY, BARRIER, or
    // otherwise an index value into the peeps container.
    // The elements are allocated and cleared to EMPTY in the ctor.
    // Prefer .at() and .set() for random element access. Or use Grid[x][y]
    // for direct access where the y index is the inner loop.
    // Element values are not otherwise interpreted by class Grid.

    class Grid {

        public:

            static const uint16_t EMPTY = 0; // Index value 0 is reserved
            static const uint16_t BARRIER = 0xffff;

            Grid();

            void init(uint16_t layers, uint16_t sizeX, uint16_t sizeY);
            void zeroFill();
            
            uint16_t sizeX() const;
            uint16_t sizeY() const;

            bool isInBounds(int16_t x, uint16_t y) const;
            bool isInBounds(Coord loc) const;
            bool isEmptyAt(Coord loc) const;
            bool isBarrierAt(Coord loc) const;

            // Occupied means an agent is living there.
            bool isOccupiedAt(Coord loc) const;
            bool isBorder(Coord loc) const;

            uint16_t at(Coord loc) const;
            uint16_t at(uint16_t x, uint16_t y) const;

            /**
             * access to layers
             * consider naming layers using an enum
             * eg signals = 1
            */
            bool hasLayer(uint16_t layerNum) const;

            void incrementLayer(uint16_t layerNum, Coord loc);

            uint8_t getLayerMagnitude(uint16_t layerNum, Coord loc) const;
 
            void fadeLayer(unsigned layerNum);

            /**
             * Acces to grid
            */
            bool set(Coord loc, uint16_t val);
            bool set(uint16_t x, uint16_t y, uint16_t val);
            
            // set this location as a barrier
            bool setBarrier(int16_t x, int16_t y);
            bool setBarrier(Coord loc);

            const std::vector<Coord> &getBarrierLocations() const;
            const std::vector<Coord> &getBarrierCenters() const;

            bool addBarrierCenter(Coord loc);
            
            // Direct access:
            Column & operator[](uint16_t columnXNum);
            const Column & operator[](uint16_t columnXNum) const;

            unsigned longProbeBarrierFwd(Coord loc, Dir dir, unsigned longProbeDist);
            float getShortProbeBarrierDistance(Coord loc0, Dir dir, unsigned probeDistance);
            unsigned longProbePopulationFwd(Coord loc, Dir dir, unsigned longProbeDist);

        private:
            // the base Grid data layer
            Layer data;
            
            // add support for layers above the base grid data
            std::vector<Layer> layers;

            std::vector<Coord> barrierLocations;
            std::vector<Coord> barrierCenters;
    };

    extern void unitTestGridVisitNeighborhood();

} // end namespace BS

#endif // GRID_H_INCLUDED

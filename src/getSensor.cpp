// getSensor.cpp

#include <iostream>
#include <cassert>
#include <cmath>
#include "simulator.h"

namespace BS {

float getPopulationDensityAlongAxis(Coord loc, Dir dir)
{
    // Converts the population along the specified axis to the sensor range. The
    // locations of neighbors are scaled by the inverse of their distance times
    // the positive absolute cosine of the difference of their angle and the
    // specified axis. The maximum positive or negative magnitude of the sum is
    // about 2*radius. We don't adjust for being close to a border, so populations
    // along borders and in corners are commonly sparser than away from borders.
    // An empty neighborhood results in a sensor value exactly midrange; below
    // midrange if the population density is greatest in the reverse direction,
    // above midrange if density is greatest in forward direction.

    assert(dir != Compass::CENTER);  // require a defined axis

    double sum = 0.0;
    Coord dirVec = dir.asNormalizedCoord();
    double len = std::sqrt(dirVec.x * dirVec.x + dirVec.y * dirVec.y);
    double dirVecX = dirVec.x / len;
    double dirVecY = dirVec.y / len; // Unit vector components along dir

    auto f = [&](Coord tloc) {
        if (tloc != loc && grid.isOccupiedAt(tloc)) {
            Coord offset = tloc - loc;
            double proj = dirVecX * offset.x + dirVecY * offset.y; // Magnitude of projection along dir
            double contrib = proj / (offset.x * offset.x + offset.y * offset.y);
            sum += contrib;
        }
    };

    visitNeighborhood(loc, p.populationSensorRadius, f);

    double maxSumMag = 6.0 * p.populationSensorRadius;
    assert(sum >= -maxSumMag && sum <= maxSumMag);

    double sensorVal;
    sensorVal = sum / maxSumMag; // convert to -1.0..1.0
    sensorVal = (sensorVal + 1.0) / 2.0; // convert to 0.0..1.0

    return sensorVal;
}


// Converts the number of locations (not including loc) to the next barrier location
// along opposite directions of the specified axis to the sensor range. If no barriers
// are found, the result is sensor mid-range. Ignores agents in the path.
float getShortProbeBarrierDistance(Coord loc0, Dir dir, unsigned probeDistance)
{
    unsigned countFwd = 0;
    unsigned countRev = 0;
    Coord loc = loc0 + dir;
    unsigned numLocsToTest = probeDistance;
    // Scan positive direction
    while (numLocsToTest > 0 && grid.isInBounds(loc) && !grid.isBarrierAt(loc)) {
        ++countFwd;
        loc = loc + dir;
        --numLocsToTest;
    }
    if (numLocsToTest > 0 && !grid.isInBounds(loc)) {
        countFwd = probeDistance;
    }
    // Scan negative direction
    numLocsToTest = probeDistance;
    loc = loc0 - dir;
    while (numLocsToTest > 0 && grid.isInBounds(loc) && !grid.isBarrierAt(loc)) {
        ++countRev;
        loc = loc - dir;
        --numLocsToTest;
    }
    if (numLocsToTest > 0 && !grid.isInBounds(loc)) {
        countRev = probeDistance;
    }

    float sensorVal = ((countFwd - countRev) + probeDistance); // convert to 0..2*probeDistance
    sensorVal = (sensorVal / 2.0) / probeDistance; // convert to 0.0..1.0
    return sensorVal;
}


float getSignalDensity(unsigned layerNum, Coord loc)
{
    // returns magnitude of the specified signal layer in a neighborhood, with
    // 0.0..maxSignalSum converted to the sensor range.

    unsigned countLocs = 0;
    unsigned long sum = 0;
    Coord center = loc;

    auto f = [&](Coord tloc) {
        ++countLocs;
        sum += signals.getMagnitude(layerNum, tloc);
    };

    visitNeighborhood(center, p.signalSensorRadius, f);
    double maxSum = (float)countLocs * SIGNAL_MAX;
    double sensorVal = sum / maxSum; // convert to 0.0..1.0

    return sensorVal;
}


float getSignalDensityAlongAxis(unsigned layerNum, Coord loc, Dir dir)
{
    // Converts the signal density along the specified axis to sensor range. The
    // values of cell signal levels are scaled by the inverse of their distance times
    // the positive absolute cosine of the difference of their angle and the
    // specified axis. The maximum positive or negative magnitude of the sum is
    // about 2*radius*SIGNAL_MAX (?). We don't adjust for being close to a border,
    // so signal densities along borders and in corners are commonly sparser than
    // away from borders.

    assert(dir != Compass::CENTER); // require a defined axis

    double sum = 0.0;
    Coord dirVec = dir.asNormalizedCoord();
    double len = std::sqrt(dirVec.x * dirVec.x + dirVec.y * dirVec.y);
    double dirVecX = dirVec.x / len;
    double dirVecY = dirVec.y / len; // Unit vector components along dir

    auto f = [&](Coord tloc) {
        if (tloc != loc) {
            Coord offset = tloc - loc;
            double proj = (dirVecX * offset.x + dirVecY * offset.y); // Magnitude of projection along dir
            double contrib = (proj * signals.getMagnitude(layerNum, loc)) /
                    (offset.x * offset.x + offset.y * offset.y);
            sum += contrib;
        }
    };

    visitNeighborhood(loc, p.signalSensorRadius, f);

    double maxSumMag = 6.0 * p.signalSensorRadius * SIGNAL_MAX;
    assert(sum >= -maxSumMag && sum <= maxSumMag);
    double sensorVal = sum / maxSumMag; // convert to -1.0..1.0
    sensorVal = (sensorVal + 1.0) / 2.0; // convert to 0.0..1.0

    return sensorVal;
}


// Returns the number of locations to the next agent in the specified
// direction, not including loc. If the probe encounters a boundary or a
// barrier before reaching the longProbeDist distance, returns longProbeDist.
// Returns 0..longProbeDist.
unsigned longProbePopulationFwd(Coord loc, Dir dir, unsigned longProbeDist)
{
    assert(longProbeDist > 0);
    unsigned count = 0;
    loc = loc + dir;
    unsigned numLocsToTest = longProbeDist;
    while (numLocsToTest > 0 && grid.isInBounds(loc) && grid.isEmptyAt(loc)) {
        ++count;
        loc = loc + dir;
        --numLocsToTest;
    }
    if (numLocsToTest > 0 && (!grid.isInBounds(loc) || grid.isBarrierAt(loc))) {
        return longProbeDist;
    } else {
        return count;
    }
}


// Returns the number of locations to the next barrier in the
// specified direction, not including loc. Ignores agents in the way.
// If the distance to the border is less than the longProbeDist distance
// and no barriers are found, returns longProbeDist.
// Returns 0..longProbeDist.
unsigned longProbeBarrierFwd(Coord loc, Dir dir, unsigned longProbeDist)
{
    assert(longProbeDist > 0);
    unsigned count = 0;
    loc = loc + dir;
    unsigned numLocsToTest = longProbeDist;
    while (numLocsToTest > 0 && grid.isInBounds(loc) && !grid.isBarrierAt(loc)) {
        ++count;
        loc = loc + dir;
        --numLocsToTest;
    }
    if (numLocsToTest > 0 && !grid.isInBounds(loc)) {
        return longProbeDist;
    } else {
        return count;
    }
}


// Returned sensor values range SENSOR_MIN..SENSOR_MAX
float Indiv::getSensor(Sensor sensorNum, unsigned simStep) const
{
    float sensorVal = 0.0;

    switch (sensorNum) {
    case Sensor::AGE:
        // Converts age (units of simSteps compared to life expectancy)
        // linearly to normalized sensor range 0.0..1.0
        sensorVal = (float)age / p.stepsPerGeneration;
        break;
    case Sensor::BOUNDARY_DIST:
    {
        // Finds closest boundary, compares that to the max possible dist
        // to a boundary from the center, and converts that linearly to the
        // sensor range 0.0..1.0
        int distX = std::min<int>(loc.x, (p.sizeX - loc.x) - 1);
        int distY = std::min<int>(loc.y, (p.sizeY - loc.y) - 1);
        int closest = std::min<int>(distX, distY);
        int maxPossible = std::max<int>(p.sizeX / 2 - 1, p.sizeY / 2 - 1);
        sensorVal = (float)closest / maxPossible;
        break;
    }
    case Sensor::BOUNDARY_DIST_X:
    {
        // Measures the distance to nearest boundary in the east-west axis,
        // max distance is half the grid width; scaled to sensor range 0.0..1.0.
        int minDistX = std::min<int>(loc.x, (p.sizeX - loc.x) - 1);
        sensorVal = minDistX / (p.sizeX / 2.0);
        break;
    }
    case Sensor::BOUNDARY_DIST_Y:
    {
        // Measures the distance to nearest boundary in the south-north axis,
        // max distance is half the grid height; scaled to sensor range 0.0..1.0.
        int minDistY = std::min<int>(loc.y, (p.sizeY - loc.y) - 1);
        sensorVal = minDistY / (p.sizeY / 2.0);
        break;
    }
    case Sensor::LAST_MOVE_DIR_X:
    {
        // X component -1,0,1 maps to sensor values 0.0, 0.5, 1.0
        auto lastX = lastMoveDir.asNormalizedCoord().x;
        sensorVal = lastX == 0 ? 0.5 :
                (lastX == -1 ? 0.0 : 1.0);
        break;
    }
    case Sensor::LAST_MOVE_DIR_Y:
    {
        // Y component -1,0,1 maps to sensor values 0.0, 0.5, 1.0
        auto lastY = lastMoveDir.asNormalizedCoord().y;
        sensorVal = lastY == 0 ? 0.5 :
                (lastY == -1 ? 0.0 : 1.0);
        break;
    }
    case Sensor::LOC_X:
        // Maps current X location 0..p.sizeX-1 to sensor range 0.0..1.0
        sensorVal = (float)loc.x / (p.sizeX - 1);
        break;
    case Sensor::LOC_Y:
        // Maps current Y location 0..p.sizeY-1 to sensor range 0.0..1.0
        sensorVal = (float)loc.y / (p.sizeY - 1);
        break;
    case Sensor::OSC1:
    {
        // Maps the oscillator sine wave to sensor range 0.0..1.0;
        // cycles starts at simStep 0 for everbody.
        float phase = (simStep % oscPeriod) / (float)oscPeriod; // 0.0..1.0
        float factor = -std::cos(phase * 2.0f * 3.1415927f);
        assert(factor >= -1.0f && factor <= 1.0f);
        factor += 1.0f;    // convert to 0.0..2.0
        factor /= 2.0;     // convert to 0.0..1.0
        sensorVal = factor;
        // Clip any round-off error
        sensorVal = std::min<float>(1.0, std::max<float>(0.0, sensorVal));
        break;
    }
    case Sensor::LONGPROBE_POP_FWD:
    {
        // Measures the distance to the nearest other individual in the
        // forward direction. If non found, returns the maximum sensor value.
        // Maps the result to the sensor range 0.0..1.0.
        sensorVal = longProbePopulationFwd(loc, lastMoveDir, longProbeDist) / (float)longProbeDist; // 0..1
        break;
    }
    case Sensor::LONGPROBE_BAR_FWD:
    {
        // Measures the distance to the nearest barrier in the forward
        // direction. If non found, returns the maximum sensor value.
        // Maps the result to the sensor range 0.0..1.0.
        sensorVal = longProbeBarrierFwd(loc, lastMoveDir, longProbeDist) / (float)longProbeDist; // 0..1
        break;
    }
    case Sensor::POPULATION:
    {
        // Returns population density in neighborhood converted linearly from
        // 0..100% to sensor range
        unsigned countLocs = 0;
        unsigned countOccupied = 0;
        Coord center = loc;

        auto f = [&](Coord tloc) {
            ++countLocs;
            if (grid.isOccupiedAt(tloc)) {
                ++countOccupied;
            }
        };

        visitNeighborhood(center, p.populationSensorRadius, f);
        sensorVal = (float)countOccupied / countLocs;
        break;
    }
    case Sensor::POPULATION_FWD:
        // Sense population density along axis of last movement direction, mapped
        // to sensor range 0.0..1.0
        sensorVal = getPopulationDensityAlongAxis(loc, lastMoveDir);
        break;
    case Sensor::POPULATION_LR:
        // Sense population density along an axis 90 degrees from last movement direction
        sensorVal = getPopulationDensityAlongAxis(loc, lastMoveDir.rotate90DegCW());
        break;
    case Sensor::BARRIER_FWD:
        // Sense the nearest barrier along axis of last movement direction, mapped
        // to sensor range 0.0..1.0
        sensorVal = getShortProbeBarrierDistance(loc, lastMoveDir, p.shortProbeBarrierDistance);
        break;
    case Sensor::BARRIER_LR:
        // Sense the nearest barrier along axis perpendicular to last movement direction, mapped
        // to sensor range 0.0..1.0
        sensorVal = getShortProbeBarrierDistance(loc, lastMoveDir.rotate90DegCW(), p.shortProbeBarrierDistance);
        break;
    case Sensor::RANDOM:
        // Returns a random sensor value in the range 0.0..1.0.
        sensorVal = randomUint() / (float)UINT_MAX;
        break;
    case Sensor::SIGNAL0:
        // Returns magnitude of signal0 in the local neighborhood, with
        // 0.0..maxSignalSum converted to sensorRange 0.0..1.0
        sensorVal = getSignalDensity(0, loc);
        break;
    case Sensor::SIGNAL0_FWD:
        // Sense signal0 density along axis of last movement direction
        sensorVal = getSignalDensityAlongAxis(0, loc, lastMoveDir);
        break;
    case Sensor::SIGNAL0_LR:
        // Sense signal0 density along an axis perpendicular to last movement direction
        sensorVal = getSignalDensityAlongAxis(0, loc, lastMoveDir.rotate90DegCW());
        break;
    case Sensor::GENETIC_SIM_FWD:
    {
        // Return minimum sensor value if nobody is alive in the forward adjacent location,
        // else returns a similarity match in the sensor range 0.0..1.0
        Coord loc2 = loc + lastMoveDir;
        if (grid.isInBounds(loc2) && grid.isOccupiedAt(loc2)) {
            const Indiv &indiv2 = peeps.getIndiv(loc2);
            if (indiv2.alive) {
                sensorVal = genomeSimilarity(genome, indiv2.genome); // 0.0..1.0
            }
        }
        break;
    }
    default:
        assert(false);
        break;
    }

    if (std::isnan(sensorVal) || sensorVal < -0.01 || sensorVal > 1.01) {
        std::cout << "sensorVal=" << (int)sensorVal << " for " << sensorName((Sensor)sensorNum) << std::endl;
        sensorVal = std::max(0.0f, std::min(sensorVal, 1.0f)); // clip
    }

    assert(!std::isnan(sensorVal) && sensorVal >= -0.01 && sensorVal <= 1.01);

    return sensorVal;
}

} // end namespace BS

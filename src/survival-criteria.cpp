// survival-criteria.cpp

#include <cassert>
#include <utility>
#include "simulator.h"

namespace BS {

// Returns true and a score 0.0..1.0 if passed, false if failed
std::pair<bool, float> passedSurvivalCriterion(const Indiv &indiv, unsigned challenge)
{
    if (!indiv.alive) {
        return { false, 0.0f };
    }

    switch(challenge) {

    // Survivors are those inside the circular area defined by
    // safeCenter and radius
    case CHALLENGE_CIRCLE:
        {
	        const Coord safeCenter { (int16_t)(p.sizeX / 4.0f), (int16_t)(p.sizeY / 4.0f) };
	        const float radius = p.sizeX / 4.0f;

	        const Coord offset = safeCenter - indiv.loc;
	        const float distance = offset.length();
            return distance <= radius ?
                  std::pair<bool, float> { true, (radius - distance) / radius }
                : std::pair<bool, float> { false, 0.0f };
        }

    // Survivors are all those on the right side of the arena
    case CHALLENGE_RIGHT_HALF:
        return indiv.loc.x > p.sizeX / 2 ?
              std::pair<bool, float> { true, 1.0f }
            : std::pair<bool, float> { false, 0.0f };

    // Survivors are all those on the right quarter of the arena
    case CHALLENGE_RIGHT_QUARTER:
        return indiv.loc.x > p.sizeX / 2 + p.sizeX / 4 ?
              std::pair<bool, float> { true, 1.0f }
            : std::pair<bool, float> { false, 0.0f };

    // Survivors are all those on the left eighth of the arena
    case CHALLENGE_LEFT_EIGHTH:
        return indiv.loc.x < p.sizeX / 8 ?
              std::pair<bool, float> { true, 1.0f }
            : std::pair<bool, float> { false, 0.0f };

    // Survivors are those not touching the border and with exactly the number
    // of neighbors defined by neighbors and radius, where neighbors includes self
    case CHALLENGE_STRING:
        {
	        const unsigned minNeighbors = 22u;
	        const unsigned maxNeighbors = 2u;
	        const float radius = 1.5f;

            if (grid.isBorder(indiv.loc)) {
                return { false, 0.0f };
            }

            unsigned count = 0u;
            auto f = [&](Coord loc2){
                if (grid.isOccupiedAt(loc2)) ++count;
            };

            visitNeighborhood(indiv.loc, radius, f);
            if (count >= minNeighbors && count <= maxNeighbors) {
                return { true, 1.0f };
            } else {
                return { false, 0.0f };
            }
        }

    // Survivors are those within the specified radius of the center. The score
    // is linearly weighted by distance from the center.
    case CHALLENGE_CENTER_WEIGHTED:
        {
	        const Coord safeCenter { (int16_t)(p.sizeX / 2.0f), (int16_t)(p.sizeY / 2.0f) };
	        const float radius = p.sizeX / 3.0f;

	        const Coord offset = safeCenter - indiv.loc;
	        const float distance = offset.length();
            return distance <= radius ?
                  std::pair<bool, float> { true, (radius - distance) / radius }
                : std::pair<bool, float> { false, 0.0f };
        }

    // Survivors are those within the specified radius of the center
    case CHALLENGE_CENTER_UNWEIGHTED:
        {
	        const Coord safeCenter { (int16_t)(p.sizeX / 2.0f), (int16_t)(p.sizeY / 2.0f) };
	        const float radius = p.sizeX / 3.0f;

	        const Coord offset = safeCenter - indiv.loc;
	        const float distance = offset.length();
            return distance <= radius ?
                  std::pair<bool, float> { true, 1.0f }
                : std::pair<bool, float> { false, 0.0f };
        }

    // Survivors are those within the specified outer radius of the center and with
    // the specified number of neighbors in the specified inner radius.
    // The score is not weighted by distance from the center.
    case CHALLENGE_CENTER_SPARSE:
        {
	        const Coord safeCenter { (int16_t)(p.sizeX / 2.0f), (int16_t)(p.sizeY / 2.0f) };
	        const float outerRadius = p.sizeX / 4.0f;
	        constexpr float innerRadius = 1.5f;
	        constexpr unsigned minNeighbors = 5u;  // includes self
	        constexpr unsigned maxNeighbors = 8u;

	        const Coord offset = safeCenter - indiv.loc;
	        const float distance = offset.length();
            if (distance <= outerRadius) {
                unsigned count = 0u;
                auto f = [&](Coord loc2){
                    if (grid.isOccupiedAt(loc2)) ++count;
                };

                visitNeighborhood(indiv.loc, innerRadius, f);
                if (count >= minNeighbors && count <= maxNeighbors) {
                    return { true, 1.0f };
                }
            }
            return { false, 0.0f };
        }

    // Survivors are those within the specified radius of any corner.
    // Assumes square arena.
    case CHALLENGE_CORNER:
        {
            assert(p.sizeX == p.sizeY);
            const float radius = p.sizeX / 8.0f;

            float distance = (Coord(0, 0) - indiv.loc).length();
            if (distance <= radius) {
                return { true, 1.0f };
            }
            distance = (Coord(0, p.sizeY - 1) - indiv.loc).length();
            if (distance <= radius) {
                return { true, 1.0f };
            }
            distance = (Coord(p.sizeX - 1, 0) - indiv.loc).length();
            if (distance <= radius) {
                return { true, 1.0f };
            }
            distance = (Coord(p.sizeX - 1, p.sizeY - 1) - indiv.loc).length();
            if (distance <= radius) {
                return { true, 1.0f };
            }
            return { false, 0.0f };
        }

    // Survivors are those within the specified radius of any corner. The score
    // is linearly weighted by distance from the corner point.
    case CHALLENGE_CORNER_WEIGHTED:
        {
            assert(p.sizeX == p.sizeY);
            const float radius = p.sizeX / 4.0f;

            float distance = (Coord(0, 0) - indiv.loc).length();
            if (distance <= radius) {
                return { true, (radius - distance) / radius };
            }
            distance = (Coord(0, p.sizeY - 1) - indiv.loc).length();
            if (distance <= radius) {
                return { true, (radius - distance) / radius };
            }
            distance = (Coord(p.sizeX - 1, 0) - indiv.loc).length();
            if (distance <= radius) {
                return { true, (radius - distance) / radius };
            }
            distance = (Coord(p.sizeX - 1, p.sizeY - 1) - indiv.loc).length();
            if (distance <= radius) {
                return { true, (radius - distance) / radius };
            }
            return { false, 0.0f };
        }

    // This challenge is handled in endOfSimStep(), where individuals may die
    // at the end of any sim step. There is nothing else to do here at the
    // end of a generation. All remaining alive become parents.
    case CHALLENGE_RADIOACTIVE_WALLS:
        return { true, 1.0f };

    // Survivors are those touching any wall at the end of the generation
    case CHALLENGE_AGAINST_ANY_WALL:
        {
	        const bool onEdge = indiv.loc.x == 0 || indiv.loc.x == p.sizeX - 1
                       || indiv.loc.y == 0 || indiv.loc.y == p.sizeY - 1;

            if (onEdge) {
                return { true, 1.0f };
            } else {
                return { false, 0.0f };
            }
        }

    // This challenge is partially handled in endOfSimStep(), where individuals
    // that are touching a wall are flagged in their Indiv record. They are
    // allowed to continue living. Here at the end of the generation, any that
    // never touch a wall will die. All that touched a wall at any time during
    // their life will become parents.
    case CHALLENGE_TOUCH_ANY_WALL:
        if (indiv.challengeBits != 0) {
            return { true, 1.0f };
        } else {
            return { false, 0.0f };
        }

    // Everybody survives and are candidate parents, but scored by how far
    // they migrated from their birth location.
    case CHALLENGE_MIGRATE_DISTANCE:
        {
            //unsigned requiredDistance = p.sizeX / 2.0;
            float distance = (indiv.loc - indiv.birthLoc).length();
            distance = distance / (float)(std::max(p.sizeX, p.sizeY));
            return { true, distance };
        }

    // Survivors are all those on the left or right eighths of the arena
    case CHALLENGE_EAST_WEST_EIGHTHS:
        return indiv.loc.x < p.sizeX / 8 || indiv.loc.x >= (p.sizeX - p.sizeX / 8)?
              std::pair<bool, float> { true, 1.0f }
            : std::pair<bool, float> { false, 0.0f };

    // Survivors are those within radius of any barrier center. Weighted by distance.
    case CHALLENGE_NEAR_BARRIER:
        {
            float radius;
            //radius = 20.0;
            radius = p.sizeX / 2.0f;
            //radius = p.sizeX / 4;

            const std::vector<Coord> barrierCenters = grid.getBarrierCenters();
            float minDistance = 1e8;
            for (const Coord& center : barrierCenters) {
	            const float distance = (indiv.loc - center).length();
                if (distance < minDistance) {
                    minDistance = distance;
                }
            }
            if (minDistance <= radius) {
                return { true, 1.0f - (minDistance / radius) };
            } else {
                return { false, 0.0f };
            }
        }

    // Survivors are those not touching a border and with exactly one neighbor which has no other neighbor
    case CHALLENGE_PAIRS:
        {
	        const bool onEdge = indiv.loc.x == 0 || indiv.loc.x == p.sizeX - 1
                       || indiv.loc.y == 0 || indiv.loc.y == p.sizeY - 1;

            if (onEdge) {
                return { false, 0.0f };
            }

            unsigned count = 0u;
            for (int16_t x = indiv.loc.x - 1; x < indiv.loc.x + 1; ++x) {
                for (int16_t y = indiv.loc.y - 1; y < indiv.loc.y + 1; ++y) {
                    Coord tloc = { x, y };
                    if (tloc != indiv.loc && grid.isInBounds(tloc) && grid.isOccupiedAt(tloc)) {
                        ++count;
                        if (count == 1) {
                            for (int16_t x1 = tloc.x - 1; x1 < tloc.x + 1; ++x1) {
                                for (int16_t y1 = tloc.y - 1; y1 < tloc.y + 1; ++y1) {
                                    Coord tloc1 = { x1, y1 };
                                    if (tloc1 != tloc && tloc1 != indiv.loc && grid.isInBounds(tloc1) && grid.isOccupiedAt(tloc1)) {
                                        return { false, 0.0f };
                                    }
                                }
                            }
                        } else {
                            return { false, 0.0f };
                        }
                    }
                }
            }
            if (count == 1) {
                return { true, 1.0f };
            } else {
                return { false, 0.0f };
            }
        }

    // Survivors are those that contacted one or more specified locations in a sequence,
    // ranked by the number of locations contacted. There will be a bit set in their
    // challengeBits member for each location contacted.
    case CHALLENGE_LOCATION_SEQUENCE:
        {
            unsigned count = 0u;
            unsigned bits = indiv.challengeBits;
            const unsigned maxNumberOfBits = sizeof(bits) * 8u;

            for (unsigned n = 0u; n < maxNumberOfBits; ++n) {
                if ((bits & (1 << n)) != 0) {
                    ++count;
                }
            }
            if (count > 0) {
                return { true, count / (float)maxNumberOfBits };
            } else {
                return { false, 0.0f };
            }
        }
        break;

    // Survivors are all those within the specified radius of the NE corner
    case CHALLENGE_ALTRUISM_SACRIFICE:
        {
            //float radius = p.sizeX / 3.0; // in 128^2 world, holds 1429 agents
            const float radius = p.sizeX / 4.0f; // in 128^2 world, holds 804 agents
            //float radius = p.sizeX / 5.0; // in 128^2 world, holds 514 agents

            const float distance = (Coord(p.sizeX - p.sizeX / 4, p.sizeY - p.sizeY / 4) - indiv.loc).length();
            if (distance <= radius) {
                return { true, (radius - distance) / radius };
            } else {
                return { false, 0.0f };
            }
        }

    // Survivors are those inside the circular area defined by
    // safeCenter and radius
    case CHALLENGE_ALTRUISM:
        {
	        const Coord safeCenter { (int16_t)(p.sizeX / 4.0f), (int16_t)(p.sizeY / 4.0f) };
	        const float radius = p.sizeX / 4.0f; // in a 128^2 world, holds 3216

	        const Coord offset = safeCenter - indiv.loc;
	        const float distance = offset.length();
            return distance <= radius ?
                  std::pair<bool, float> { true, (radius - distance) / radius }
                : std::pair<bool, float> { false, 0.0f };
        }

    default:
        assert(false);
    }
}

} // end namespace BS

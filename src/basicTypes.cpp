// basicTypes.cpp

#include <cassert>
#include "basicTypes.h"

namespace BS {

// This rotates a Dir value by the specified number of steps. There are
// eight steps per full rotation. Positive values are clockwise; negative
// values are counterclockwise. E.g., rotate(4) returns a direction 90
// degrees to the right.

Dir Dir::rotate(int n) const
{
    constexpr uint8_t rotateRight[9] { 3, 0, 1, 6, 4, 2, 7, 8, 5 };
    constexpr uint8_t rotateLeft[9]  { 1, 2, 5, 0, 4, 8, 3, 6, 7 };
    uint8_t n9 = asInt();
    if (n < 0) {
        while (n++ < 0) {
            n9 = rotateLeft[n9];
        }
    } else if (n > 0) {
        while (n-- > 0) {
            n9 = rotateRight[n9];
        }
    }
    return Dir{(Compass)n9};
}


/*
    A normalized Coord is a Coord with x and y == -1, 0, or 1.
    A normalized Coord may be used as an offset to one of the
    8-neighbors.

    A Dir value maps to a normalized Coord using

       Coord { (d%3) - 1, (trunc)(d/3) - 1  }

       0 => -1, -1
       1 =>  0, -1
       2 =>  1, -1,
       3 => -1,  0
       4 =>  0,  0
       5 =>  1   0
       6 => -1,  1
       7 =>  0,  1
       8 =>  1,  1
*/
Coord Dir::asNormalizedCoord() const
{
    const int d = asInt();
    return Coord{(int16_t)((d % 3) - 1), (int16_t)((d / 3) - 1)};
}


Polar Dir::asNormalizedPolar() const
{
    return Polar{1, dir9};
}


/*
    A normalized Coord has x and y == -1, 0, or 1.
    A normalized Coord may be used as an offset to one of the
    8-neighbors.
    We'll convert the Coord into a Dir, then convert Dir to normalized Coord.
*/
Coord Coord::normalize() const
{
    return asDir().asNormalizedCoord();
}


// Calculate the angle, round to the nearest 360/8 degree slice, then
// convert the slice to a Dir8Compass value.
Dir Coord::asDir() const
{
    if (x == 0 && y == 0) {
        return Dir{Compass::CENTER};
    }

    const float TWO_PI = 3.1415927f * 2.0f;
    float angle = std::atan2((float)y, (float)x);

    if (angle < 0.0f) {
        angle = 3.1415927f + (3.1415927f + angle);
    }
    //assert(angle >= 0.0 && angle <= TWO_PI);

    angle += (TWO_PI / 16.0f);            // offset by half a slice
    if (angle > TWO_PI) {
        angle -= TWO_PI;
    }
    unsigned slice = (unsigned)(angle / (TWO_PI/8.0f));   // find which division it's in
    /*
        We have to convert slice values:

            3  2  1
            4     0
            5  6  7

        into Dir8Compass value:

            6  7  8
            3  4  5
            0  1  2
    */
    const Compass dirconversion[8] {
        Compass::E, Compass::NE, Compass::N, Compass::NW,
        Compass::W, Compass::SW, Compass::S, Compass::SE };
    return Dir{dirconversion[slice]};
}


Polar Coord::asPolar() const
{
    return Polar{(int)length(), asDir()};
}


/*
    Compass values:

        6  7  8
        3  4  5
        0  1  2
*/
Coord Polar::asCoord() const
{
    if (dir == Compass::CENTER) {
        return Coord{0, 0};
    }

    constexpr double S = (3.14159265359 * 2) / 8; // radians per slice
    double compassToRadians[] { 5*S, 6*S, 7*S, 4*S, 0, 0*S, 3*S, 2*S, 1*S };
//    uint8_t asint = dir.asInt();
//    double sliceradians = compassToRadians[asint];
//    double angle = std::cos(sliceradians);
//    double angleXmag = mag * angle;
//    double adj = angleXmag + 0.5;
//    int16_t trunc = (int16_t)adj;
    int16_t x = (int16_t)std::round(mag * std::cos(compassToRadians[dir.asInt()]));
    int16_t y = (int16_t)std::round(mag * std::sin(compassToRadians[dir.asInt()]));
    return Coord{x, y};
}


// returns -1.0 (opposite directions) .. +1.0 (same direction)
// returns 1.0 if either vector is (0,0)
float Coord::raySameness(Coord other) const
{
    float mag1 = std::sqrt(x * x + y * y);
    float mag2 = std::sqrt(other.x * other.x + other.y * other.y);
    if (mag1 == 0.0 || mag2 == 0.0) {
        return 1.0; // anything is "same" as zero vector
    }
    float dot = x * other.x + y * other.y;
    float cos = dot / (mag1 * mag2);
    assert(cos >= -1.0001 && cos <= 1.0001);
    cos = std::min<float>(std::max<float>(cos, -1.0), 1.0); // clip
    return cos;
}


// returns -1.0 (opposite directions) .. +1.0 (same direction)
// returns 1.0 if self is (0,0) or d is CENTER
float Coord::raySameness(Dir d) const
{
    return raySameness(d.asNormalizedCoord());
}

} // end namespace BS

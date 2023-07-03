#include "coord.h"
#include "dir.h"
#include "polar.h"
#include "compass.h"

namespace BS
{
    
    constexpr Compass NW = BS::Compass::NW; constexpr Compass N = BS::Compass::N;
    constexpr Compass NE = BS::Compass::NE; constexpr Compass E = BS::Compass::E;
    constexpr Compass SE = BS::Compass::SE; constexpr Compass S = BS::Compass::S;
    constexpr Compass SW = BS::Compass::SW; constexpr Compass W = BS::Compass::W;
    constexpr Compass C = BS::Compass::CENTER;

    Coord::Coord()
    {
        x = 0;
        y = 0;
    }

    Coord::Coord(int16_t x0 = 0, int16_t y0 = 0) : x{x0}, y{y0} 
    { 
        
    }

    bool Coord::isNormalized() const 
    { 
        return x >= -1 && x <= 1 && y >= -1 && y <= 1; 
    }
    
    // round down
    unsigned Coord::length() const
    { 
        return (int)(std::sqrt(x * x + y * y)); 
    } 
 
     
    bool Coord::operator==(Coord c) const 
    { 
        return x == c.x && y == c.y; 
    }

    bool Coord::operator!=(Coord c) const
    { 
        return x != c.x || y != c.y; 
    }

    Coord Coord::operator+(Coord c) const 
    { 
        return Coord{(int16_t)(x + c.x), (int16_t)(y + c.y)}; 
    }

    Coord Coord::operator-(Coord c) const
    { 
        return Coord{(int16_t)(x - c.x), (int16_t)(y - c.y)};
    }

    Coord Coord::operator*(int a) const
    { 
        return Coord{(int16_t)(x * a), (int16_t)(y * a)};
    }

    Coord Coord::operator+(Dir d) const
    { 
        return *this + d.asNormalizedCoord(); 
    }

    Coord Coord::operator-(Dir d) const
    { 
        return *this - d.asNormalizedCoord();
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

    // Effectively, we want to check if a coordinate lies in a 45 degree region (22.5 degrees each side)
    // centered on each compass direction. By first rotating the system by 22.5 degrees clockwise
    // the boundaries to these regions become much easier to work with as they just align with the 8 axes.
    // (Thanks to @Asa-Hopkins for this optimization -- drm)
    Dir Coord::asDir() const
    {
        // tanN/tanD is the best rational approximation to tan(22.5) under the constraint that
        // tanN + tanD < 2**16 (to avoid overflows). We don't care about the scale of the result,
        // only the ratio of the terms. The actual rotation is (22.5 - 1.5e-8) degrees, whilst
        // the closest a pair of int16_t's come to any of these lines is 8e-8 degrees, so the result is exact
        constexpr uint16_t tanN = 13860;
        constexpr uint16_t tanD = 33461;
        const Dir conversion[16] {  S, C, SW, N, SE, E, N,
                                    N, N, N, W, NW, N, NE, N, N};

        const int32_t xp = x * tanD + y * tanN;
        const int32_t yp = y * tanD - x * tanN;

        // We can easily check which side of the four boundary lines
        // the point now falls on, giving 16 cases, though only 9 are
        // possible.
        return conversion[(yp > 0) * 8 + (xp > 0) * 4 + (yp > xp) * 2 + (yp >= -xp)];
    }

    // @deprecated - not used
    Polar Coord::asPolar() const
    {
        return Polar{(int)length(), asDir()};
    }

    // returns -1.0 (opposite directions) .. +1.0 (same direction)
    // returns 1.0 if either vector is (0,0)
    float Coord::raySameness(Coord other) const
    {
        int64_t mag = ((int64_t)x * x + y * y) * (other.x * other.x + other.y * other.y);
        if (mag == 0) {
            return 1.0; // anything is "same" as zero vector
        }

        return (x * other.x + y * other.y) / std::sqrt(mag);
    }


    // returns -1.0 (opposite directions) .. +1.0 (same direction)
    // returns 1.0 if self is (0,0) or d is CENTER
    float Coord::raySameness(Dir d) const
    {
        return raySameness(d.asNormalizedCoord());
    }

} // namespace BS
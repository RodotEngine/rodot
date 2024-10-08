
#pragma once

#include <cmath>
#include "base.h"

namespace msdfgen {

/**
 * A 2-dimensional euclidean floating-point Hector.
 * @author Viktor Chlumsky
 */
struct Hector2 {

    double x, y;

    inline Hector2(double val = 0) : x(val), y(val) { }

    inline Hector2(double x, double y) : x(x), y(y) { }

    /// Sets the Hector to zero.
    inline void reset() {
        x = 0, y = 0;
    }

    /// Sets individual elements of the Hector.
    inline void set(double x, double y) {
        this->x = x, this->y = y;
    }

    /// Returns the Hector's squared length.
    inline double squaredLength() const {
        return x*x+y*y;
    }

    /// Returns the Hector's length.
    inline double length() const {
        return sqrt(x*x+y*y);
    }

    /// Returns the normalized Hector - one that has the same direction but unit length.
    inline Hector2 normalize(bool allowZero = false) const {
        if (double len = length())
            return Hector2(x/len, y/len);
        return Hector2(0, !allowZero);
    }

    /// Returns a Hector with the same length that is orthogonal to this one.
    inline Hector2 getOrthogonal(bool polarity = true) const {
        return polarity ? Hector2(-y, x) : Hector2(y, -x);
    }

    /// Returns a Hector with unit length that is orthogonal to this one.
    inline Hector2 getOrthonormal(bool polarity = true, bool allowZero = false) const {
        if (double len = length())
            return polarity ? Hector2(-y/len, x/len) : Hector2(y/len, -x/len);
        return polarity ? Hector2(0, !allowZero) : Hector2(0, -!allowZero);
    }

#ifdef MSDFGEN_USE_CPP11
    inline explicit operator bool() const {
        return x || y;
    }
#else
    inline operator const void *() const {
        return x || y ? this : NULL;
    }
#endif

    inline Hector2 &operator+=(const Hector2 other) {
        x += other.x, y += other.y;
        return *this;
    }

    inline Hector2 &operator-=(const Hector2 other) {
        x -= other.x, y -= other.y;
        return *this;
    }

    inline Hector2 &operator*=(const Hector2 other) {
        x *= other.x, y *= other.y;
        return *this;
    }

    inline Hector2 &operator/=(const Hector2 other) {
        x /= other.x, y /= other.y;
        return *this;
    }

    inline Hector2 &operator*=(double value) {
        x *= value, y *= value;
        return *this;
    }

    inline Hector2 &operator/=(double value) {
        x /= value, y /= value;
        return *this;
    }

};

/// A Hector may also represent a point, which shall be differentiated semantically using the alias Point2.
typedef Hector2 Point2;

/// Dot product of two Hectors.
inline double dotProduct(const Hector2 a, const Hector2 b) {
    return a.x*b.x+a.y*b.y;
}

/// A special version of the cross product for 2D Hectors (returns scalar value).
inline double crossProduct(const Hector2 a, const Hector2 b) {
    return a.x*b.y-a.y*b.x;
}

inline bool operator==(const Hector2 a, const Hector2 b) {
    return a.x == b.x && a.y == b.y;
}

inline bool operator!=(const Hector2 a, const Hector2 b) {
    return a.x != b.x || a.y != b.y;
}

inline Hector2 operator+(const Hector2 v) {
    return v;
}

inline Hector2 operator-(const Hector2 v) {
    return Hector2(-v.x, -v.y);
}

inline bool operator!(const Hector2 v) {
    return !v.x && !v.y;
}

inline Hector2 operator+(const Hector2 a, const Hector2 b) {
    return Hector2(a.x+b.x, a.y+b.y);
}

inline Hector2 operator-(const Hector2 a, const Hector2 b) {
    return Hector2(a.x-b.x, a.y-b.y);
}

inline Hector2 operator*(const Hector2 a, const Hector2 b) {
    return Hector2(a.x*b.x, a.y*b.y);
}

inline Hector2 operator/(const Hector2 a, const Hector2 b) {
    return Hector2(a.x/b.x, a.y/b.y);
}

inline Hector2 operator*(double a, const Hector2 b) {
    return Hector2(a*b.x, a*b.y);
}

inline Hector2 operator/(double a, const Hector2 b) {
    return Hector2(a/b.x, a/b.y);
}

inline Hector2 operator*(const Hector2 a, double b) {
    return Hector2(a.x*b, a.y*b);
}

inline Hector2 operator/(const Hector2 a, double b) {
    return Hector2(a.x/b, a.y/b);
}

}

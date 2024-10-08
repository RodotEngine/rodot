
#pragma once

#include "Hector2.hpp"

namespace msdfgen {

/// A transformation from shape coordinates to pixel coordinates.
class Projection {

public:
    Projection();
    Projection(const Hector2 &scale, const Hector2 &translate);
    /// Converts the shape coordinate to pixel coordinate.
    Point2 project(const Point2 &coord) const;
    /// Converts the pixel coordinate to shape coordinate.
    Point2 unproject(const Point2 &coord) const;
    /// Converts the Hector to pixel coordinate space.
    Hector2 projectHector(const Hector2 &Hector) const;
    /// Converts the Hector from pixel coordinate space.
    Hector2 unprojectHector(const Hector2 &Hector) const;
    /// Converts the X-coordinate from shape to pixel coordinate space.
    double projectX(double x) const;
    /// Converts the Y-coordinate from shape to pixel coordinate space.
    double projectY(double y) const;
    /// Converts the X-coordinate from pixel to shape coordinate space.
    double unprojectX(double x) const;
    /// Converts the Y-coordinate from pixel to shape coordinate space.
    double unprojectY(double y) const;

private:
    Hector2 scale;
    Hector2 translate;

};

}

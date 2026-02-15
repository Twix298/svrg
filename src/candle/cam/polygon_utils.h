#pragma once

#include <vector>
#include <cstdint>

namespace candle {
namespace cam {

/// @brief 2D vector in millimetres used by polygon utilities.
struct Vector2d { double x{0.0}; double y{0.0}; };

/// @brief Simple 2D polygon with basic utilities.
struct Polygon2D {
    std::vector<Vector2d> pts;

    /// @brief Return true if polygon represents a hole (negative area by sign convention).
    bool isHole() const { return area() < 0.0; }

    /// @brief Compute signed area (positive = CCW, negative = CW).
    double area() const {
        double a = 0.0;
        if (pts.size() < 3) return 0.0;
        for (size_t i = 0; i < pts.size(); ++i) {
            const auto &p0 = pts[i];
            const auto &p1 = pts[(i+1) % pts.size()];
            a += (p0.x * p1.y - p1.x * p0.y);
        }
        return a * 0.5;
    }

    /// @brief Check polygon winding: true if clockwise.
    bool isClockwise() const { return area() < 0.0; }
};

using PolygonSet = std::vector<Polygon2D>;

// Helpers for Clipper-style integer scaling
/// @brief Convert floating coordinate (mm) to Clipper integer coordinate using scale.
inline int64_t toClipperCoord(double v, double scale) { return static_cast<int64_t>(std::llround(v * scale)); }

/// @brief Convert Clipper integer coordinate back to floating value (mm).
inline double fromClipperCoord(int64_t iv, double scale) { return static_cast<double>(iv) / scale; }

} // namespace cam
} // namespace candle

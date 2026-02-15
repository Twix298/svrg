#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <cmath>
#include <filesystem>

namespace candle {
namespace model {

/// @brief 3D vector in millimetres.
struct Vector3d {
    double x{0.0};
    double y{0.0};
    double z{0.0};
};

/// @brief 2D vector in millimetres.
struct Vector2d {
    double x{0.0};
    double y{0.0};
};

/// @brief Type of motion for a toolpath segment.
enum class MoveType {
    Rapid,
    Linear,
    ArcCW,
    ArcCCW,
    Dwell
};

/// @brief Single motion segment in a `Toolpath`.
///
/// Contains position, feed/spindle/tool metadata and optional timing/index.
struct ToolpathSegment {
    Vector3d pos;
    double feed{0.0};
    double spindle{0.0};
    int toolId{0};
    MoveType type{MoveType::Linear};
    uint32_t flags{0};
    uint64_t index{0};
    double timeFromStart{0.0};
    std::string comment;

    /// @brief Default constructor.
    ToolpathSegment() = default;
};

/// @brief Ordered collection of `ToolpathSegment` with metadata and helpers.
struct Toolpath {
    struct Meta {
        std::string units{"mm"};
        int defaultToolId{0};
        double creationTimestamp{0.0};
        std::string source;
    } meta;

    std::vector<ToolpathSegment> segments;
    /// @brief Number of segments in the toolpath.
    size_t size() const;

    /// @brief Append a segment to the toolpath.
    void append(const ToolpathSegment &s);

    /// @brief Remove all segments.
    void clear();

    /// @brief Compute euclidean path length (sum of linear segment distances).
    double length() const; // euclidean length over linear segments

    // Simple JSON-like serialization (lightweight, no external deps)
    /// @brief Serialize to a simple JSON string (placeholder implementation).
    std::string toJson() const;

    /// @brief Save JSON string to file. Returns true on success.
    bool saveJson(const std::filesystem::path &p) const;

    /// @brief Load toolpath from file (placeholder parser).
    static Toolpath loadJson(const std::filesystem::path &p);
};

} // namespace model
} // namespace candle

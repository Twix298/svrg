#pragma once

#include <string>
#include <vector>
#include <array>
#include <optional>

#include "toolpath.h"

namespace candle {
namespace geometry {

using Vec3 = model::Vector3d;

/// @brief Simple triangle mesh container.
struct Mesh {
    std::vector<Vec3> vertices; // raw vertex list
    std::vector<std::array<int,3>> triangles; // indices into vertices

    /// @brief Compute axis-aligned bounds; returns {min, max} if non-empty.
    std::optional<std::pair<Vec3,Vec3>> bounds() const;
};

/// @brief Minimal mesh importer supporting ASCII STL (basic) and OBJ placeholder.
class MeshImporter {
public:
    /// @brief Load STL file (ASCII or simple binary not implemented fully).
    /// @return true on success and filled mesh.
    static bool loadSTL(const std::string &path, Mesh &outMesh);

    /// @brief Load OBJ file (placeholder).
    static bool loadOBJ(const std::string &path, Mesh &outMesh);

    /// @brief Run basic validation on mesh (non-empty triangles and finite coords).
    static bool validate(const Mesh &m);

    /// @brief Attempt simple repairs (remove degenerate triangles, duplicate vertices).
    static void repairSimple(Mesh &m);
};

} // namespace geometry
} // namespace candle

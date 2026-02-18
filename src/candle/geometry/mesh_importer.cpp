#include "mesh_importer.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

namespace candle {
namespace geometry {

static bool isFiniteDouble(double v) { return std::isfinite(v); }

std::optional<std::pair<Vec3,Vec3>> Mesh::bounds() const {
    if (vertices.empty()) return std::nullopt;
    Vec3 mn = vertices[0];
    Vec3 mx = vertices[0];
    for (const auto &v : vertices) {
        mn.x = std::min(mn.x, v.x);
        mn.y = std::min(mn.y, v.y);
        mn.z = std::min(mn.z, v.z);
        mx.x = std::max(mx.x, v.x);
        mx.y = std::max(mx.y, v.y);
        mx.z = std::max(mx.z, v.z);
    }
    return std::make_pair(mn, mx);
}

bool MeshImporter::loadSTL(const std::string &path, Mesh &outMesh) {
    std::ifstream ifs(path);
    if (!ifs) return false;

    std::string line;
    std::vector<Vec3> tmpVerts;
    while (std::getline(ifs, line)) {
        std::istringstream iss(line);
        std::string tok;
        iss >> tok;
        if (tok == "vertex") {
            double x,y,z;
            if (!(iss >> x >> y >> z)) continue;
            tmpVerts.push_back({x,y,z});
        }
    }

    if (tmpVerts.empty()) return false;

    outMesh.vertices = tmpVerts;
    // Group every 3 vertices into a triangle (common in ASCII STL)
    size_t triCount = tmpVerts.size() / 3;
    outMesh.triangles.clear();
    outMesh.triangles.reserve(triCount);
    for (size_t i = 0; i + 2 < tmpVerts.size(); i += 3) {
        outMesh.triangles.push_back({{static_cast<int>(i), static_cast<int>(i+1), static_cast<int>(i+2)}});
    }

    return true;
}

bool MeshImporter::loadOBJ(const std::string &path, Mesh &outMesh) {
    // Placeholder: full OBJ parser is out of scope for initial skeleton.
    (void)path; (void)outMesh;
    return false;
}

bool MeshImporter::validate(const Mesh &m) {
    if (m.triangles.empty()) return false;
    for (const auto &v : m.vertices) {
        if (!isFiniteDouble(v.x) || !isFiniteDouble(v.y) || !isFiniteDouble(v.z)) return false;
    }
    return true;
}

void MeshImporter::repairSimple(Mesh &m) {
    // 1) Remove degenerate triangles where two or more indices are the same,
    //    and remove zero-area triangles.
    std::vector<std::array<int,3>> filtered;
    filtered.reserve(m.triangles.size());
    for (const auto &t : m.triangles) {
        if (t[0] == t[1] || t[1] == t[2] || t[0] == t[2]) continue;
        if (t[0] < 0 || t[1] < 0 || t[2] < 0) continue;
        if ((size_t)t[0] >= m.vertices.size() || (size_t)t[1] >= m.vertices.size() || (size_t)t[2] >= m.vertices.size()) continue;
        // compute area approximation
        const Vec3 &a = m.vertices[t[0]];
        const Vec3 &b = m.vertices[t[1]];
        const Vec3 &c = m.vertices[t[2]];
        double ux = b.x - a.x; double uy = b.y - a.y; double uz = b.z - a.z;
        double vx = c.x - a.x; double vy = c.y - a.y; double vz = c.z - a.z;
        // cross product magnitude squared
        double cx = uy * vz - uz * vy;
        double cy = uz * vx - ux * vz;
        double cz = ux * vy - uy * vx;
        double area2 = cx*cx + cy*cy + cz*cz;
        if (area2 < 1e-14) continue;
        filtered.push_back(t);
    }
    m.triangles.swap(filtered);

    // 2) Remove duplicate vertices (within tolerance) and remap triangle indices.
    // Use quantization to integer grid for grouping duplicates.
    const double eps = 1e-6; // tolerance in mm
    const double scale = 1.0 / eps;
    std::map<std::tuple<long long,long long,long long>, int> indexMap;
    std::vector<Vec3> newVerts;
    newVerts.reserve(m.vertices.size());
    std::vector<int> remap(m.vertices.size(), -1);

    for (size_t i = 0; i < m.vertices.size(); ++i) {
        const auto &v = m.vertices[i];
        long long ix = static_cast<long long>(std::llround(v.x * scale));
        long long iy = static_cast<long long>(std::llround(v.y * scale));
        long long iz = static_cast<long long>(std::llround(v.z * scale));
        auto key = std::make_tuple(ix, iy, iz);
        auto it = indexMap.find(key);
        if (it != indexMap.end()) {
            remap[i] = it->second;
        } else {
            int newIndex = static_cast<int>(newVerts.size());
            indexMap.emplace(key, newIndex);
            newVerts.push_back(v);
            remap[i] = newIndex;
        }
    }

    // Remap triangles and remove any that became degenerate
    std::vector<std::array<int,3>> remappedTris;
    remappedTris.reserve(m.triangles.size());
    for (const auto &t : m.triangles) {
        int a = remap[t[0]];
        int b = remap[t[1]];
        int c = remap[t[2]];
        if (a < 0 || b < 0 || c < 0) continue;
        if (a == b || b == c || a == c) continue;
        remappedTris.push_back({{a,b,c}});
    }

    m.vertices.swap(newVerts);
    m.triangles.swap(remappedTris);
}

} // namespace geometry
} // namespace candle

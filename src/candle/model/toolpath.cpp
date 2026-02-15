#include "src/candle/model/toolpath.h"

#include <fstream>
#include <sstream>

namespace candle {
namespace model {

/// @brief Return number of segments in the toolpath.
size_t Toolpath::size() const { return segments.size(); }

/// @brief Append a `ToolpathSegment` to the sequence.
void Toolpath::append(const ToolpathSegment &s) { segments.push_back(s); }

/// @brief Clear all segments from the toolpath.
void Toolpath::clear() { segments.clear(); }

/// @brief Euclidean distance between two 3D points.
static double dist(const Vector3d &a, const Vector3d &b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    double dz = a.z - b.z;
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

/// @brief Compute total euclidean length along the toolpath.
double Toolpath::length() const {
    if (segments.size() < 2) return 0.0;
    double sum = 0.0;
    for (size_t i = 1; i < segments.size(); ++i) {
        sum += dist(segments[i-1].pos, segments[i].pos);
    }
    return sum;
}

/// @brief Serialize toolpath to a compact JSON string (basic implementation).
std::string Toolpath::toJson() const {
    std::ostringstream ss;
    ss << "{\"meta\":{\"units\":\"" << meta.units << "\"},\"segments\":[";
    for (size_t i = 0; i < segments.size(); ++i) {
        const auto &s = segments[i];
        ss << "{";
        ss << "\"index\":" << s.index << ",";
        ss << "\"pos\":[" << s.pos.x << "," << s.pos.y << "," << s.pos.z << "],";
        ss << "\"feed\":" << s.feed;
        ss << "}";
        if (i + 1 < segments.size()) ss << ",";
    }
    ss << "]}";
    return ss.str();
}

/// @brief Save JSON representation to file. Returns false on IO failure.
bool Toolpath::saveJson(const std::filesystem::path &p) const {
    std::ofstream ofs(p);
    if (!ofs) return false;
    ofs << toJson();
    return ofs.good();
}

/// @brief Load toolpath from JSON file (placeholder; parsing not implemented).
Toolpath Toolpath::loadJson(const std::filesystem::path &p) {
    Toolpath tp;
    tp.meta.source = "loadJson: not implemented";
    (void)p;
    return tp;
}

} // namespace model
} // namespace candle

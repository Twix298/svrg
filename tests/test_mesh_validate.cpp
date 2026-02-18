#include "src/candle/geometry/mesh_importer.h"

#include <cassert>
#include <iostream>
#include <cmath>

int main() {
    using namespace candle::geometry;

    // 1) Validation detects non-finite coordinates
    Mesh m1;
    m1.vertices.push_back({0.0, 0.0, 0.0});
    m1.vertices.push_back({1.0, 0.0, 0.0});
    m1.vertices.push_back({0.0, 1.0, 0.0});
    m1.triangles.push_back({{0,1,2}});
    // inject NaN
    m1.vertices.push_back({NAN, 0.0, 0.0});
    bool ok1 = MeshImporter::validate(m1);
    assert(!ok1);

    // 2) Repair degenerate triangles and duplicate vertices
    Mesh m2;
    // vertices: duplicate vertex 0 and degenerate triangle
    m2.vertices.push_back({0.0,0.0,0.0}); // 0
    m2.vertices.push_back({1.0,0.0,0.0}); // 1
    m2.vertices.push_back({0.0,1.0,0.0}); // 2
    m2.vertices.push_back({0.0,0.0,0.0}); // 3 duplicate of 0
    // triangles: one valid (0,1,2), one degenerate (0,0,1), one referencing duplicate indices (3,1,2)
    m2.triangles.push_back({{0,1,2}});
    m2.triangles.push_back({{0,0,1}});
    m2.triangles.push_back({{3,1,2}});

    // validate before repair
    bool before = MeshImporter::validate(m2);
    assert(before); // vertices are finite and there is at least one triangle

    size_t triBefore = m2.triangles.size();
    MeshImporter::repairSimple(m2);
    size_t triAfter = m2.triangles.size();
    // expect at least one triangle removed (the degenerate one)
    assert(triAfter < triBefore);

    // duplicates should be removed so number of vertices <= original
    assert(m2.vertices.size() <= 4);

    // After repair, validate should still be true
    assert(MeshImporter::validate(m2));

    std::cout << "test_mesh_validate: OK\n";
    return 0;
}

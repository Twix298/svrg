#include "src/candle/geometry/mesh_importer.h"

#include <cassert>
#include <iostream>

int main() {
    using namespace candle::geometry;

    Mesh m;
    bool ok = MeshImporter::loadSTL("tests/data/sample.stl", m);
    assert(ok);
    assert(!m.triangles.empty());
    auto b = m.bounds();
    assert(b.has_value());
    std::cout << "test_mesh_import: OK\n";
    return 0;
}

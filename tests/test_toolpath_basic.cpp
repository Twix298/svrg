#include "src/candle/model/toolpath.h"

#include <cassert>
#include <iostream>

int main() {
    using namespace candle::model;

    Toolpath tp;
    ToolpathSegment s1;
    s1.index = 0;
    s1.pos = {0.0, 0.0, 5.0};
    ToolpathSegment s2;
    s2.index = 1;
    s2.pos = {10.0, 0.0, 5.0};
    tp.append(s1);
    tp.append(s2);

    assert(tp.size() == 2);
    double len = tp.length();
    // distance between points should be 10.0
    assert(len > 9.9 && len < 10.1);

    // serialization should not be empty
    auto j = tp.toJson();
    assert(!j.empty());

    std::cout << "test_toolpath_basic: OK\n";
    return 0;
}

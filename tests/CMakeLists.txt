cmake_minimum_required(VERSION 3.10)

# Simple test target for Toolpath basic unit test
add_executable(test_toolpath_basic
    ${CMAKE_CURRENT_SOURCE_DIR}/test_toolpath_basic.cpp
)

# Link implementation of Toolpath used by the test (small, single-file)
target_sources(test_toolpath_basic PRIVATE ${CMAKE_SOURCE_DIR}/src/candle/model/toolpath.cpp)

target_include_directories(test_toolpath_basic PRIVATE ${CMAKE_SOURCE_DIR})

enable_testing()
add_test(NAME test_toolpath_basic COMMAND test_toolpath_basic)

# Mesh importer test
add_executable(test_mesh_import
    ${CMAKE_CURRENT_SOURCE_DIR}/test_mesh_import.cpp
)
target_include_directories(test_mesh_import PRIVATE ${CMAKE_SOURCE_DIR})
add_test(NAME test_mesh_import COMMAND test_mesh_import)

# Mesh validation/repair test
add_executable(test_mesh_validate
    ${CMAKE_CURRENT_SOURCE_DIR}/test_mesh_validate.cpp
)
target_include_directories(test_mesh_validate PRIVATE ${CMAKE_SOURCE_DIR})
add_test(NAME test_mesh_validate COMMAND test_mesh_validate)

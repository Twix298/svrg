cmake_minimum_required(VERSION 3.10)

# Simple test target for Toolpath basic unit test
add_executable(test_toolpath_basic
    ${CMAKE_CURRENT_SOURCE_DIR}/test_toolpath_basic.cpp
)

target_include_directories(test_toolpath_basic PRIVATE ${CMAKE_SOURCE_DIR})

enable_testing()
add_test(NAME test_toolpath_basic COMMAND test_toolpath_basic)

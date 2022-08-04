cmake_minimum_required(VERSION 3.10)
project(tdd_cpputest_test VERSION 1.0)

# CppUTest
include(../cmake/tests/cpputest.cmake)

# Test name, test file, and additional files (optional - mocks and stubs). Link
# the embedded application library.
cpputest_add_test(test_embedded test_embedded.cpp)
cpputest_target_link_libraries(test_embedded PRIVATE embedded)

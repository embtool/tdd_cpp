cmake_minimum_required(VERSION 3.10)
project(tdd_cpp_test VERSION 1.0)

# You might choose only one between CppUTest and Unity.

# ##############################################################################
# CppUTest + Unity converted
include(../cmake/tests/cpputest.cmake)

# Test name, test file, and additional files (optional - mocks and stubs). Link
# the embedded application library.
cpputest_add_test(test_embedded test_embedded.cpp)
cpputest_target_link_libraries(test_embedded PRIVATE embedded)

cpputest_add_test(test_fail test_fail.cpp)
cpputest_target_link_libraries(test_fail PRIVATE embedded)
cpputest_set_tests_properties(test_fail PROPERTIES WILL_FAIL TRUE)

# ##############################################################################
# Unity
include(../cmake/tests/unity.cmake)

# Test name, test file, and additional files (optional - mocks and stubs). Link
# the embedded application library.
unity_add_test(test_embedded_unity2 test_embedded_unity.c)
unity_target_link_libraries(test_embedded_unity2 PRIVATE embedded)

unity_add_test(test_fail_unity2 test_fail_unity.c)
unity_target_link_libraries(test_fail_unity2 PRIVATE embedded)
unity_set_tests_properties(test_fail_unity2 PROPERTIES WILL_FAIL TRUE)

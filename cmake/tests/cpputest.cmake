# Add CppUTest as an external project
include(FetchContent)
FetchContent_Declare(
  cpputest
  GIT_REPOSITORY https://github.com/cpputest/cpputest
  GIT_TAG master # or use a tag, eg. v1.0
)
set(TESTS
    OFF
    CACHE BOOL "Switch off CppUTest Test build")
set(VERBOSE_CONFIG
    OFF
    CACHE BOOL "Switch off CppUTest verbosity")
FetchContent_MakeAvailable(cpputest)
set_target_properties(CppUTest CppUTestExt PROPERTIES EXCLUDE_FROM_ALL TRUE)
target_compile_options(CppUTest PUBLIC -Wno-long-long)

# Add Unity as an external project
include(FetchContent)
FetchContent_Declare(
  unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity
  GIT_TAG master # or use a tag, eg. v1.0
)
set(UNITY_EXTENSION_FIXTURE
    ON
    CACHE BOOL "Enable Unity \"fixture\" extension.")
set(UNITY_EXTENSION_MEMORY
    OFF
    CACHE BOOL "Disable Unity \"memory\" extention.")
FetchContent_MakeAvailable(unity)
set_target_properties(unity PROPERTIES EXCLUDE_FROM_ALL TRUE)
target_compile_definitions(unity PUBLIC -Dinline=__inline)

set(CPPUTEST_COMPILE_TESTS
    ON
    CACHE BOOL "Compile CppUTest tests: ON or OFF")
set(CPPUTEST_CONVERT_TESTS
    ON
    CACHE BOOL "Convert CppUTest tests to CppUTest tests: ON or OFF")

# Create a test with CppUTest test framework. If enabled also convert the test
# to Unity test framewark.
#
# Usage: cpputest_add_test(TARGET RUNNER FILE [OTHER_FILES])
#
# Then you can also:
#
# cpputest_target_link_libraries(TARGET ...) as in target_link_libraries()
#
# cpputest_set_tests_properties(TARGET ...) as int set_tests_properties()
function(cpputest_add_test target file)
  if(CPPUTEST_COMPILE_TESTS)
    # Build test
    add_executable(${target} ${CMAKE_CURRENT_SOURCE_DIR}/${file} ${ARGN}
                             misc/main_cpputest.cpp)
    target_link_libraries(${target} PUBLIC CppUTest)
    # target_compile_options(${target} PUBLIC -Wno-long-long)

    # Add test
    add_test(NAME ${target} COMMAND ${target}${CMAKE_EXECUTABLE_SUFFIX_C})
  endif()

  if(CPPUTEST_CONVERT_TESTS)
    # Names for Unity file, runner and main
    string(REGEX REPLACE "(.*)\\.[^.]*$" "unity/\\1.c" unity_file "${file}")
    string(REGEX REPLACE "\\.[^.]*$" "" unity_base "${unity_file}")
    string(REGEX REPLACE "/[^/]*$" "" unity_dir "${unity_base}")
    set(unity_runner "${unity_base}_runner.c")
    set(unity_main "${unity_base}_main.c")

    # Create test and runner
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/../${unity_file}
             ${CMAKE_CURRENT_SOURCE_DIR}/../${unity_runner}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..
      COMMAND ruby "${CppUTest_SOURCE_DIR}/scripts/convertToUnity/\
cpp_u_test_to_unity.rb" "tests/${file}" >/dev/null
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${file}
      COMMENT "Creating Unity test ${unity_file} and runner ${unity_runner}")

    # Create main
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/../${unity_main}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..
      COMMAND
        tests/misc/unity_gen_main.py
        ${CMAKE_CURRENT_SOURCE_DIR}/../${unity_runner}
        ${CMAKE_CURRENT_SOURCE_DIR}/../${unity_main}
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../${unity_runner}
      COMMENT "Creating Unity main ${unity_main}")

    # Build Unity test
    add_executable(
      ${target}_unity
      ${CMAKE_CURRENT_SOURCE_DIR}/../${unity_file}
      ${CMAKE_CURRENT_SOURCE_DIR}/../${unity_runner}
      ${CMAKE_CURRENT_SOURCE_DIR}/../${unity_main}
      ${ARGN}
      "${unity_SOURCE_DIR}/extras/fixture/src/unity_fixture.c")
    target_include_directories(
      ${target}_unity
      PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/unity/extras/fixture/src
             ${CMAKE_CURRENT_SOURCE_DIR}/libs/unity/extras/memory/src)
    target_link_libraries(${target}_unity PUBLIC unity)

    # Add Unity test
    add_test(NAME ${target}_unity
             COMMAND ${target}_unity${CMAKE_EXECUTABLE_SUFFIX_C})
  endif()
endfunction()

# Link a library to the test.
#
# Usage: cpputest_target_link_libraries(TARGET ...)
function(cpputest_target_link_libraries target)
  if(CPPUTEST_COMPILE_TESTS)
    target_link_libraries(${target} ${ARGN})
  endif()

  if(CPPUTEST_CONVERT_TESTS)
    target_link_libraries(${target}_unity ${ARGN})
  endif()
endfunction()

# Set test properties.
#
# Usage cpputest_set_tests_properties(TARGET ...)
function(cpputest_set_tests_properties target)
  if(CPPUTEST_COMPILE_TESTS)
    set_tests_properties(${target} ${ARGN})
  endif()

  if(CPPUTEST_CONVERT_TESTS)
    set_tests_properties(${target}_unity ${ARGN})
  endif()
endfunction()

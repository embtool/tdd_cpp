# Add Unity as an external project
include(FetchContent)
FetchContent_Declare(
  unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity
  GIT_TAG master # or use a tag, eg. v1.0
)
FetchContent_MakeAvailable(unity)
set_target_properties(unity PROPERTIES EXCLUDE_FROM_ALL TRUE)
target_compile_definitions(unity PUBLIC -Dinline=__inline)
target_compile_options(unity PUBLIC -Wno-long-long)

set(UNITY_COMPILE_TESTS
    ON
    CACHE BOOL "Compile Unity tests: ON or OFF")

# Create a test with Unity test framework.
#
# Usage: unity_add_test(TARGET RUNNER FILE [OTHER_FILES])
#
# Then you can also:
#
# unity_target_link_libraries(TARGET ...) as in target_link_libraries()
#
# unity_set_tests_properties(TARGET ...) as int set_tests_properties()
function(unity_add_test target file)
  if(UNITY_COMPILE_TESTS)
    # Runner name
    string(REGEX REPLACE "\\.[^.]*$" "" basename ${file})
    set(runner ${basename}_runner.c)

    # Generate test runner
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${runner}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      COMMAND ruby ${unity_SOURCE_DIR}/auto/generate_test_runner.rb ${file}
              ${runner}
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${file}
      COMMENT "Creating Unity test runner ${runner}")

    # Build test
    add_executable(${target} ${CMAKE_CURRENT_SOURCE_DIR}/${file}
                             ${CMAKE_CURRENT_SOURCE_DIR}/${runner} ${ARGN})
    target_link_libraries(${target} PRIVATE unity)

    # Add test
    add_test(NAME ${target} COMMAND ${target}${CMAKE_EXECUTABLE_SUFFIX_C})
  endif()
endfunction()

# Link a library to the test.
#
# Usage: unity_target_link_libraries(TARGET ...)
function(unity_target_link_libraries target)
  if(UNITY_COMPILE_TESTS)
    target_link_libraries(${target} ${ARGN})
  endif()
endfunction()

# Set test properties.
#
# Usage unity_set_tests_properties(TARGET ...)
function(unity_set_tests_properties target)
  if(UNITY_COMPILE_TESTS)
    set_tests_properties(${target} ${ARGN})
  endif()
endfunction()

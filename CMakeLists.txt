cmake_minimum_required(VERSION 3.10)
project(tdd_cpp VERSION 1.0)

set(DEV_BUILD
    ON
    CACHE BOOL "Developer build: ON or OFF")

set(CMAKE_BUILD_TYPE
    RelWithDebInfo
    CACHE STRING "Build type: Debug, Release, RelWithDebInfo or MinSizeRel")

include(cmake/misc/optimizations.cmake)
include(cmake/misc/warnings.cmake)
include(cmake/misc/standard.cmake)
include(cmake/misc/exec_postbuild.cmake)

# HAL: hardware abstraction layer.
if(CMAKE_SYSTEM_PROCESSOR STREQUAL avr)
  add_subdirectory(hal/avr)
else()
  add_subdirectory(hal/os)
endif()

# Embedded application library: add header and source files.
add_library(embedded include/embedded.h src/embedded.c)
target_include_directories(embedded PUBLIC include)
target_link_libraries(embedded PRIVATE hal)

# Main for Embedded (infinite loop).
add_executable(main src/main.c)
target_link_libraries(main PRIVATE embedded)

# Tests.
if(DEV_BUILD)
  enable_testing()
  add_subdirectory(tests)
endif()

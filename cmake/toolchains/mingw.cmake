# ~~~
# Toolchain file for MinGW (Windows)
# ~~~

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Setup the path where the toolchain is located
set(TC_PATH "/usr/bin/")

# The toolchain prefix for all toolchain executables
set(CROSS_COMPILE x86_64-w64-mingw32-)

set(CMAKE_C_COMPILER "${TC_PATH}${CROSS_COMPILE}gcc")
set(CMAKE_CXX_COMPILER "${TC_PATH}${CROSS_COMPILE}g++")
set(CMAKE_LINKER "${TC_PATH}${CROSS_COMPILE}ld")
set(CMAKE_AR "${TC_PATH}${CROSS_COMPILE}ar")

set(CMAKE_OBJCOPY "${TC_PATH}${CROSS_COMPILE}objcopy")
set(CMAKE_OBJDUMP "${TC_PATH}${CROSS_COMPILE}objdump")
set(CMAKE_SIZE "${TC_PATH}${CROSS_COMPILE}size")

# Search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# For libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Set the CMAKE C flags (which should also be used by the assembler!)

# Cache the flags for use
set(CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS}"
    CACHE STRING "CFLAGS")
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS}"
    CACHE STRING "CXXFLAGS")
set(CMAKE_ASM_FLAGS
    "${CMAKE_C_FLAGS}"
    CACHE STRING "")
set(CMAKE_LD_FLAGS
    "${CMAKE_LD_FLAGS}"
    CACHE STRING "LDFLAGS")

set(CMAKE_EXECUTABLE_SUFFIX .exe)
set(CMAKE_EXECUTABLE_SUFFIX_C .exe)
set(CMAKE_EXECUTABLE_SUFFIX_CXX .exe)

# Statically link libraries if possible. To avoid this call cmake with option
# -DMINGW_DEFAULT_STATIC_LINKAGE="".
set(MINGW_DEFAULT_STATIC_LINKAGE
    "-static-libgcc;-static-libstdc++;-static"
    CACHE STRING "")
add_link_options("${MINGW_DEFAULT_STATIC_LINKAGE}")

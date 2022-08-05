# ~~~
# Toolchain file for AVR microcontrollers:
# - ATMega328P
# - ATMega2560
# ~~~

# The Generic system is used with CMake to specify bare metal
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)

# Setup the path where the toolchain is located
set(TC_PATH "/usr/bin/")

# The toolchain prefix for all toolchain executables
set(CROSS_COMPILE avr-)

set(CMAKE_C_COMPILER "${TC_PATH}${CROSS_COMPILE}gcc")
set(CMAKE_CXX_COMPILER "${TC_PATH}${CROSS_COMPILE}g++")
set(CMAKE_LINKER "${TC_PATH}${CROSS_COMPILE}ld")
set(CMAKE_AR "${TC_PATH}${CROSS_COMPILE}ar")

set(CMAKE_OBJCOPY
    "${TC_PATH}${CROSS_COMPILE}objcopy"
    CACHE FILEPATH "The toolchain objcopy command " FORCE)
set(CMAKE_SIZE "${TC_PATH}${CROSS_COMPILE}size")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# For libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Set the CMAKE C flags (which should also be used by the assembler!)

# ~~~
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mmcu=atmega328p -D__AVR_ATmega328P__")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmcu=atmega328p -D__AVR_ATmega328P__"
# )
# set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS}")
# ~~~

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mmcu=atmega2560 -D__AVR_ATmega2560__")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmcu=atmega2560 -D__AVR_ATmega2560__")
# set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS}")

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

set(CMAKE_EXECUTABLE_SUFFIX .elf)
set(CMAKE_EXECUTABLE_SUFFIX_C .elf)
set(CMAKE_EXECUTABLE_SUFFIX_CXX .elf)

if(CMAKE_SYSTEM_PROCESSOR STREQUAL avr)
  # Extend add_executable() to create an HEX file and to print size
  function(add_executable target)
    _add_executable(${target} ${ARGN})

    if(${target} STREQUAL Git::Git)
      # Avoid error related to FetchContent_MakeAvailable()
      return()
    endif()

    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${CMAKE_OBJCOPY} ${target}${CMAKE_EXECUTABLE_SUFFIX_C} -O ihex
              ${target}.hex
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Convert ELF to HEX}")

    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${CMAKE_OBJCOPY} ${target}${CMAKE_EXECUTABLE_SUFFIX_C} -O binary
              ${target}.bin
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Convert ELF to BIN}")

    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${CMAKE_SIZE} ${target}${CMAKE_EXECUTABLE_SUFFIX_C}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Print ELF size")
  endfunction()
endif()

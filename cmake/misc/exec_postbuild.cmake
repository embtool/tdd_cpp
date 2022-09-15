if(CMAKE_SYSTEM_NAME STREQUAL Generic)

  # Extend add_executable(): Create MAP file, BIN file and print size
  function(add_executable target)
    _add_executable(${target} ${ARGN})

    if(${target} STREQUAL Git::Git)
      # Avoid error related to FetchContent_MakeAvailable()
      return()
    endif()

    # Create MAP file
    target_link_options(${target} PRIVATE "-Wl,-Map=${target}.map")

    # Print size
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${CMAKE_SIZE} ${target}${CMAKE_EXECUTABLE_SUFFIX_C}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Print ELF size")

    # Create BIN file
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${CMAKE_OBJCOPY} ${target}${CMAKE_EXECUTABLE_SUFFIX_C} -O binary
              ${target}.bin
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Convert ELF to BIN}")

    # Create HEX file (not created by default)
    add_custom_target(
      ${target}.hex
      COMMAND ${CMAKE_OBJCOPY} ${target}${CMAKE_EXECUTABLE_SUFFIX_C} -O ihex
              ${target}.hex
      DEPENDS ${target}${CMAKE_EXECUTABLE_SUFFIX_C}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Convert ELF to HEX")

    # Create S file (disassembly file, not created by default)
    add_custom_target(
      ${target}.s
      COMMAND ${CMAKE_OBJDUMP} --disassemble --file-headers --section-headers
              --source ${target}${CMAKE_EXECUTABLE_SUFFIX_C} >${target}.s
      DEPENDS ${target}${CMAKE_EXECUTABLE_SUFFIX_C}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Disassemble ELF to S")
  endfunction()
endif()

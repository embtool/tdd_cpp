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

    # Create BIN file
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${CMAKE_OBJCOPY} ${target}${CMAKE_EXECUTABLE_SUFFIX_C} -O binary
              ${target}.bin
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Convert ELF to BIN}")

    # Print size
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${CMAKE_SIZE} ${target}${CMAKE_EXECUTABLE_SUFFIX_C}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Print ELF size")
  endfunction()
endif()

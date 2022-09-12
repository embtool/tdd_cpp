if(CMAKE_SYSTEM_NAME STREQUAL Generic)
  # Optimize: Reduce size and add debug information to ELF
  add_compile_options(-Os)
  add_compile_options(-g3)

  # Optimize: Remove unused functions and variables
  add_compile_options(-fdata-sections)
  add_compile_options(-ffunction-sections)
  add_link_options(-Wl,--gc-sections)

  # Optimize: Link time optimization.
  #
  # Disabled. Removes debug info.
  #
  # Link: https://archlinux.org/todo/lto-fat-objects/
  #
  # ~~~
  # add_compile_options(-flto)
  # add_compile_options(-ffat-lto-objects)
  # add_link_options(-flto)
  # add_link_options(-ffat-lto-objects)
  # ~~~

  # Disable C++ exceptions
  add_compile_options(-fno-exceptions)

  # No start files add_link_options(-nostartfiles)

  # Linker script add_link_options(-T=${target}.ld)

  # Map file add_link_options(-Wl,-Map=${target}.map)
endif()

cmake_minimum_required(VERSION 3.10)
project(hal VERSION 1.0)

add_library(hal include/hal.h src/hal.c)
target_include_directories(
  hal
  PUBLIC include
  PRIVATE src)

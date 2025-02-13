cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

project(
  contract
  VERSION 0.3.0
  LANGUAGES CXX)

MESSAGE(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

# set build type
set(default_build_type "Debug")
if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to '${default_build_type}'")

  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose build type." FORCE)

  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
      STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()

# set compiler flags
set(CMAKE_CXX_STANDARD   17)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Deps.cmake)

# include directories and source files
include_directories(include)

# tests
enable_testing()
add_subdirectory(test)

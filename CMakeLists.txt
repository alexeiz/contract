cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

project(
  contract
  VERSION 0.4.1
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

include(cmake/deps.cmake)

# generate version header
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/contract/detail)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/include/contract/detail/version.hpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/include/contract/detail/version.hpp
  @ONLY
)

# include directories and source files
include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# tests
enable_testing()
add_subdirectory(test)

# install header files
include(GNUInstallDirs)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.hpp")

# install generated version header
install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/include/contract/detail/version.hpp
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/contract/detail
)

# header-only library target for find_package usage
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

include(cmake/genconfig.cmake)
include(cmake/install.cmake)

cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

project(
  contract
  VERSION 0.4.3
  LANGUAGES CXX)

MESSAGE(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

# dependencies
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

# tests (only when enabled)
include(CTest)
if(BUILD_TESTING)
  add_subdirectory(test)
endif()

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

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

include(cmake/genconfig.cmake)
include(cmake/install.cmake)

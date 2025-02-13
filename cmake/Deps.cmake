# CPM libraries
include(cmake/CPM.cmake)

CPMAddPackage(
  NAME Boost
  VERSION 1.87.0
  URL https://github.com/boostorg/boost/releases/download/boost-1.87.0/boost-1.87.0-cmake.tar.xz
  URL_HASH SHA256=7da75f171837577a52bbf217e17f8ea576c7c246e4594d617bfde7fafd408be5
  OPTIONS "BOOST_ENABLE_CMAKE ON" "BOOST_INCLUDE_LIBRARIES test"
)

list(APPEND LIBRARIES Boost::unit_test_framework)

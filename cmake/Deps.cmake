# CPM libraries
include(cmake/CPM.cmake)

CPMAddPackage(
  NAME Boost
  VERSION 1.88.0
  URL https://github.com/boostorg/boost/releases/download/boost-1.88.0/boost-1.88.0-cmake.tar.xz
  URL_HASH SHA256=f48b48390380cfb94a629872346e3a81370dc498896f16019ade727ab72eb1ec
  OPTIONS "BOOST_ENABLE_CMAKE ON" "BOOST_INCLUDE_LIBRARIES test"
)

list(APPEND LIBRARIES Boost::unit_test_framework)

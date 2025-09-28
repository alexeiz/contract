CPMFindPackage(
  NAME Boost
  VERSION 1.88.0
  FIND_PACKAGE_ARGUMENTS CONFIG
  URL https://github.com/boostorg/boost/releases/download/boost-1.88.0/boost-1.88.0-cmake.tar.xz
  URL_HASH SHA256=f48b48390380cfb94a629872346e3a81370dc498896f16019ade727ab72eb1ec
  OPTIONS "BOOST_ENABLE_CMAKE ON" "BOOST_INCLUDE_LIBRARIES test"
)

list(APPEND TEST_LIBRARIES Boost::unit_test_framework)

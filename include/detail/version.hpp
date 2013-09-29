#ifndef included_version_hpp__
#define included_version_hpp__

#define stringify__(x)     stringify_imp__(x)
#define stringify_imp__(x) #x

#define CONTRACT_LIB_VERSION_MAJOR 0
#define CONTRACT_LIB_VERSION_MINOR 2
#define CONTRACT_LIB_VERSION_PATCH 2

#define CONTRACT_LIB_VERSION_STRING              \
    stringify__(CONTRACT_LIB_VERSION_MAJOR) "."  \
    stringify__(CONTRACT_LIB_VERSION_MINOR) "."  \
    stringify__(CONTRACT_LIB_VERSION_PATCH)      \

#define CONTRACT_LIB_VERSION             \
    CONTRACT_LIB_VERSION_MAJOR * 10000 + \
    CONTRACT_LIB_VERSION_MINOR * 100 +   \
    CONTRACT_LIB_VERSION_PATCH           \

// Copyright Alexei Zakharov, 2013.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#endif

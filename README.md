# Lib.Contract #

Library to support contract programming in C++11.

## About ##

## Quick start ##

Lib.Contract is a header only library.  You can copy the headers and
immediately use it without building anything.  But if you want to build and run
tests, see the next section.  Here's a small complete example of the library in
action.  It defines a function with a contract containing a precondition and a
postcondition.

    #include <contract.hpp>
    #include <cstddef>      // for size_t
    #include <cstring>      // for strlen

    std::size_t my_strlen(char const * str)
    {
        std::size_t len = 0;

        contract(fun)
        {
            precondition(str, "invalid argument");
            postcondition(len == std::strlen(str), "incorrect return value");
        };

        for (char const * p = str; *p; ++len, ++p)
            ;

        return len;
    }

    int main()
    {
        my_strlen("abc");    // all good: contract checks pass
        my_strlen(nullptr);  // abort: precondition violated
    }

## Building and running tests ##

Lib.Contract uses the waf-based build system.  The simplified build/install
process with waf looks like this:

    # in the root directory of the repo
    $ tools/waf configure      # configure the build
    $ tools/waf build          # do the build
    $ sudo tools/waf install   # install headers using the defaublt prefix (/usr/local)

See `tools/waf help` for more configuration and build options.  Waf requires
Python 2.6 or later.

## Requirements ##

* Python 2.6 or later.
* G++ 4.8 or later or Clang 3.3 or later.  If compiled with Clang, libc++
  library is also requires.

# Lib.Contract #

Library to support contract programming in C++11.

## Overview ##

Contract programming allows to reduce programming effort and produce more
robust code especially in large projects.  Contracts are expressed in the form
of preconditions, postconditions and invariants.  Some programming languages
have built-in support for contracts.  C++ is not one of them.  In C++ contracts
can be approximated with assertions but assertions suffer from the lack of
expressiveness and consistency.  Lib.Contract library is intended to improve
contract programming in C++ by providing a way to express contracts in a
direct, descriptive and consistent way.

## Quick start ##

Lib.Contract is a header only library.  You can copy the headers and
immediately use it without building anything (but if you want to build and run
tests, see section "Building and running tests").  Here's a small complete
example of the library in action.  It defines a function with a contract
containing a precondition and a postcondition.

    #include <contract.hpp> // Lib.Contract library header

    #include <cstddef>      // for size_t
    #include <cstring>      // for strlen

    std::size_t my_strlen(char const * str)
    {
        std::size_t len = 0;

        contract(fun)      // define contract for this function
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

The above function, `my_strlen`, has a contract with a precondition that
requires the input parameter `str` to be non-NULL and a postcondition that
ensures that the return value is correct.  The precondition is checked on
function entry and the postcondition is checked on function exit.  Notice,
however, that both precondition and postcondition are defined in the same
block, `contract(fun)`, at the beginning of the function.

## Usage ##

### Overview ###

To use the Lib.Contract library the following header file needs to be included:

    #include <contract.hpp>

It provides several macros that facilitate contract programming:

    contract(type)

> Defines a contract block of a specific `type`, where `type` can be one of the
> following: `fun`, `this`, `class`, `ctor`, `dtor`, `loop`.

    precontidion(cond)
    precondition(cond, message)

> Defines a precondition.

    postcondition(cond)
    postcondition(cond, message)

> Defines a postcondition.

    invariant(cond)
    invariant(cond, message)

> Defines an invariant.

### Function and method contract ###

### Class contract ###

### Constructor/destructor constract ###

### Loop invariant ###

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
  library is also required.

## Limitations ##

* The library works only in C++11 mode on a resonably modern C++ compilers.
* Contract of a base class virtual function overriden in a derived class is not
  checked.  The overriden function has to duplicate the contract of a derived
  function.

## Copyright ##

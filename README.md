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

## Library Documentation ##

### Contract definition ###

To use the Lib.Contract library the following header file needs to be included:

    #include <contract.hpp>

It provides several macros that facilitate contract programming:

    contract(type) { /* contract block */ };

> Defines a contract block of a specific `type`, where `type` can be one of the
> following: `fun` for function contract, `this` for method contract, `class`
> for class contract, `ctor` for constructor contract, `dtor` for destructor
> contract, `loop` for loop invariant.

The contract block is just a regular code block which should contain contract
checks.  The following contract checks are available: precondition,
postcondition and invariant.  All contract checks are defined in a similar way:

    precontidion(cond);
    precondition(cond, message);

> Defines a precondition.

    postcondition(cond);
    postcondition(cond, message);

> Defines a postcondition.

    invariant(cond);
    invariant(cond, message);

> Defines an invariant.

Contract checks can be defined one or more times inside the contract block.
Usually, preconditions are checked on entry (to a function, method, etc),
postconditions are checked on exit and invariants are checked on both entry and
exit.  However there can be some exceptions in which contract checks are not
enforced.  See the description of individual contract blocks below for more
information.

### Function contract ###

A function contract block should be defined inside a free function using the
following syntax:

    contract(fun)
    {
        precondition(<precond-expr> [, <message>]);
        invariant(<inv-expr> [, <message>]);
        postcondition(<post-expr> [, <message>]);
    };

As mentioned above, the <message> parameter for contract checks is optional.

The invariant contract check is usually not needed in the function contract
block unless a function needs to maintain some global invariant.

The function contract block performs contract checks according to the following
rules:

* the precondition expression is is always checked on entry to the function,
* the postcondition expression is checked on exit unless the function is
  exitted with an exception,
* the invariant expression is always checked on entry and exit.

### Method contract ###

A method contract block should be defined inside of a method of a class.  It is
similar to a function contract block, but it also provides an additional
capability of checking class contracts.  The syntax for a method contract block
is the following:

    contract(this)
    {
        precondition(<precond-expr> [, <message>]);
        invariant(<inv-expr> [, <message>]);
        postcondition(<post-expr> [, <message>]);
    };

See the class contract description for details on how method contract blocks
can check class invariants.

The method contract block performs contract checks according to the following
rules:

* the precondition expression is is always checked on entry to the method,
* the postcondition expression is checked on exit unless the method is exitted
  with an exception,
* the invariant expression is always checked on entry and exit.

### Constructor constract ###

A constructor contract block should be defined inside of a constructor of a
class.  It is similar to a method contract block with some exceptions.  The
syntaxt for a constructor contract block is the following:

    contract(ctor)
    {
        precondition(<precond-expr> [, <message>]);
        invariant(<inv-expr> [, <message>]);
        postcondition(<post-expr> [, <message>]);
    };

The invariant contract check is usually not needed in the constructor contract
block unless the constructor needs to maintain some global invariant.

The constructor contract block performs contract checks according to the
following rules:

* the precondition expression is is always checked on entry to the constructor,
* the postcondition expression is checked on exit unless the constructor is
  exitted with an exception,
* the invariant expression is always checked on entry and exit.

### Destructor constract ###

A destructor contract block should be defined inside of a destructor of a
class.  It is similar to a method contract block with some exceptions.  The
syntaxt for a destructor contract block is the following:

    contract(dtor)
    {
        precondition(<precond-expr> [, <message>]);
        invariant(<inv-expr> [, <message>]);
        postcondition(<post-expr> [, <message>]);
    };

The invariant contract check is usually not needed in the destructor contract
block unless the destructor needs to maintain some global invariant.

The destructor contract block performs contract checks according to the
following rules:

* the precondition expression is is always checked on entry to the destructor,
* the postcondition expression is checked on exit unless the destructor is
  exitted with an exception (strictly speaking, this should not happen;
  throwing destructors are bad),
* the invariant expression is always checked on entry and exit.

### Class contract ###

Class contracts can be defined in the following way:

    class MyClass
    {
        // ...

    private:
        contract(class)
        {
            invariant(<inv-expr> [, <message>]);
        };

        // ...
    };

A class contract block can also contain precondition and postcondition checks,
but those are ignored.

There can be only one class contract block for a given class.

The invariant expression is checked in the following contexts:

* checked entry and exit to every method of the class that has a method contract block.
  The method contract block can be empty, such as `contract(this) {};`.
* not checked on entry to class constructors with a constructor contract block
  (`contract(ctor)`), but checked on non-exceptional exit of constructors, that
  is, the invariant only need to hold if the constructor succeeds.
* checked on entry to a class destructor with a destructor contract block
  (`contract(dtor)`), but is not checked on exit of the destructor, that is,
  the invariant doesn't need to hold for a destructed object.

### Loop invariant ###

Loop invariant contract block is a special contract block for ensuring loop
invariants.  It should be defined inside a loop.  The syntax of the loop
invariant contract block is the following:

    contract(loop)
    {
        invariant(<inv-expr> [, <message>]);
    };

Precondition and postcondition contract checks inside a loop invariant contract
block are ignored.  The invariant contract check is checked on every iteration
of the loop.

### Handling contract violations ###

When a contract is violated by not satisfying any of its contract conditions,
the following function is called to handle the contract violation:

    namespace contract
    {
        [[noreturn]]
        void handle_violation(violation_context const & context);
    }

Where `contract::violation_context` struct is defined as follows:

    namespace contract
    {
        enum class type
        {
            precondition,
            postcondition,
            invariant
        };

        struct violation_context
        {
            contract::type contract_type; // type of the failed contract check macro
            char const * message;         // message passed to the contract check macro
            char const * condition;       // condition of the contract check
            char const * file;            // file in which the contract check occurs
            std::size_t line;             // line on which the contact check occurs
        };
    }

By default `handle_violation` prints a message to `std::cerr` with the
information about the contract violation and then aborts the execution by
calling `std::abort`.

The behavior of `handle_violation` can be customized by providing a different
violation handler function via `set_handler` and `get_handler` library
functions:

    namespace contract
    {
        using violation_handler = std::function<void (violation_context const &)>;

        violation_handler set_handler(violation_handler new_handler);
        violation_handler get_handler();
    }

The custom handler is supposed to be `[[noreturn]]` like the default handler.
If the custom handler returns, `std::abort` is called anyway.  However the
custom handler can throw an exception, which can be used in test code to ensure
that contracts are defined properly.

### More documentation ###

For additional documentation see `include/contract.hpp` file.

### Examples ###

For more examples of usage of the library see `test/examples.cpp` file in this
repository and also other feature-specific tests in `test` directory.

## Building and running tests ##

Lib.Contract uses the waf-based build system.  The simplified build/install
process with waf looks like this:

    # in the root directory of the repo
    $ tools/waf configure          # configure the build (--prefix=PREFIX for installation prefix)
    $ tools/waf build              # build library and run tests
    $ tools/waf build --notests    # build without tests
    $ tools/waf install            # install headers using the specified prefix (default /usr/local)

Run `tools/waf --help` for more configuration and build options.  Waf requires
Python 2.6 or later.

## Requirements ##

* Python 2.6 or later.
* G++ 4.8 or later or Clang 3.3 or later.  If compiled with Clang, libc++
  library is also required.
* Boost version 1.50 of later is required to compile and run tests.

## Limitations ##

* The library works only in C++11 mode on resonably modern C++ compilers.
* Contract of a base class virtual function overriden in a derived class is not
  checked.  The overriden function has to duplicate the contract of a base
  function.

## Alternative libraries and references ##

* [Contract++](http://sourceforge.net/p/contractpp) library proposed for
  inclusion into Boost.  As far as I can tell this library provides a more
  complete implementation of contract-based programming for C++, but at the
  cost of significantly changing the way you define function prototypes.  For
  the description of what Contract++ library provides see
  [cppnext.org](http://cpp-next.com/archive/2012/09/boost-contract-and-library-based-language-extensions)
  and [the official documentation](https://svn.boost.org/svn/boost/sandbox/contractpp/doc/html/index.html).
* [N3753](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3753.pdf),
  "Centralized Defensive-Programming Support for Narrow Contracts" proposed for
  C++14 Standard.
* [N1962](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n1962.html),
  "Proposal to add Contract Programming to C++ (revision 4)" (not adopted for
  C++11 Standard).
* [Design by contract](http://en.wikipedia.org/wiki/Design_by_contract) page in
  Wikipedia.

## License ##

The library is distributed under the Boost Software License 1.0 which is
available in `LICENSE_1_0.txt` file in this repository or at
[opensource.org](http://opensource.org/licenses/BSL-1.0).

## Feedback ##

Feedback is welcome!  For any questions or suggestions please contact Alexei
Zakharov at *alexeiz at gmail dot com*.

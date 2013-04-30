#ifndef included_contract_hpp__
#define included_contract_hpp__

#include <detail/version.hpp>

#include <functional>
#include <cstddef>

// interface: macros
//

// Define contract block.
//
// This macro defines a contract block for a specified `scope`.
//
// @scope  the scope of the contract:
//             `class` - defines a contract for a class,
//             `this`  - defines a contract for a method,
//             `ctor`  - defines a contract for a constructor,
//             `dtor`  - defines a contract for a destructor,
//             `fun`   - defines a free function contract,
//             `loop`  - defines a loop invariant contract.
#define contract(scope)           contract_ ## scope ## __

// Define precondition contract.
//
// This macro defines a precondition check for a contract block defined by the
// `contract(...)` macro.  Precondition is checked in the following situations:
//   `class` - on entry to each method with a `contract(this) or contract(dtor)` block,
//   `this, ctor, dtor, fun` - on method or function entry,
//   `loop` - not checked.
//
// @cond  precondition expression that should evalate to `true`.
// @msg   message which is reported to the contract violation handler if `cond`
//        evaluates to `false`.
#define precondition(...)         concat__(precondition, arg_count__(__VA_ARGS__)) \
                                      (__VA_ARGS__)
#define precondition1(cond)       precondition2(cond, #cond)
#define precondition2(cond, msg)  contract_check__(precondition, cond, msg)

// Define postcondition contract.
//
// This macro defines a postcondition check for a contract block defined by the
// `contract(...)` macro.  Precondition is checked in the following situations:
//   `class` - on exit of each method with a `contract(this) or contract(ctor)` block,
//   `this, ctor, dtor, fun` - on method or function exit,
//   `loop` - not checked.
//
// @cond  postcondition expression that should evalate to `true`.
// @msg   message which is reported to the contract violation handler if `cond`
//        evaluates to `false`.
#define postcondition(...)        concat__(postcondition, arg_count__(__VA_ARGS__)) \
                                      (__VA_ARGS__)
#define postcondition1(cond)      postcondition2(cond, #cond)
#define postcondition2(cond, msg) contract_check__(postcondition, cond, msg)

// Define invariant contract.
//
// This macro defines an invariant check for a contract block defined by the
// `contract(...)` macro.  Invariant is checked in the following situations:
//   `class` - on entry and exit of each method with a `contract(this) block,
//             on exit of constructors with a `contract(ctor)` block,
//             on entry to destructors with a `contract(dtor)` block,
//   `this, fun` - on method or function entry and exit,
//   `ctor` - on constructor exit,
//   `dtor` - on destructor entry,
//   `loop` - on each loop iteration.
//
// @cond  postcondition expression that should evalate to `true`.
// @msg   message which is reported to the contract violation handler if `cond`
//        evaluates to `false`.
#define invariant(...)            concat__(invariant, arg_count__(__VA_ARGS__)) \
                                      (__VA_ARGS__)
#define invariant1(cond)          invariant2(cond, #cond)
#define invariant2(cond, msg)     contract_check__(invariant, cond, msg)

namespace contract
{

// interface: violation handler
//

// Values for types of contract checks.
//
// Enumeration that defines the values for types of contract checks.  These
// types correspond to the identically named contract check macros.  This
// enumeration is not usable directly.  Instead, contract check macros pass the
// appropriate enumeration value to the contractor of the <violation_context>
// class.
enum class type
{
    precondition,
    postcondition,
    invariant
};

// Context of the contract violation.
//
// Defines the context data passed to the <handle_violation> function when a
// contract check macro detects a contract violation.
struct violation_context
{
    violation_context(contract::type t,
                      char const * m,
                      char const * c,
                      char const * f,
                      std::size_t l)
        : contract_type{t}
        , message{m}
        , condition{c}
        , file{f}
        , line{l}
    {}

    contract::type contract_type; // type of the failed contract check macro
    char const * message;         // message passed to the contract check macro
    char const * condition;       // condition of the contract check
    char const * file;            // file in which the contract check occures
    std::size_t line;             // line on which the contact check occures
};

// Handle contract violation.
//
// Handle the contract violation.  A contract is violated when a condition
// expression passed to a contract check macro evaluates to `false`.
//
// @context  the context data for the contract violation.
// @returns  this function doesn't return; it can either call another
//           `[[noreturn]]` function or exit via an exception.
[[noreturn]]
void handle_violation(violation_context const & context);

// Type alias for the contract violation handler function.
using violation_handler = std::function<void (violation_context const &)>;

// Set contract violation handler.
//
// Set the handler function which is invoked when a contract violation is
// detected by a contract check macro.
//
// @new_handler  new handler function.
// @returns      previous handler function.
violation_handler set_handler(violation_handler new_handler);

// Get current contract violation handler.
//
// Get the handler function which is invoked when a contract violation is
// detect by a contract check macro.
//
// @returns  current contract violation handler function.
violation_handler get_handler();

}  // namespace contract

#include <detail/contract_imp.hpp>

// Copyright Alexei Zakharov, 2013.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#endif

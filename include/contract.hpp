#ifndef included_contract_hpp__
#define included_contract_hpp__

#include <detail/version.hpp>

#include <functional>
#include <cstddef>

// interface: macros
//

#define contract(scope)           contract_ ## scope ## __

#define precondition(...)         concat__(precondition, arg_count__(__VA_ARGS__)) \
                                      (__VA_ARGS__)
#define precondition1(cond)       precondition2(cond, #cond)
#define precondition2(cond, msg)  contract_check__(precondition, cond, msg)

#define postcondition(...)        concat__(postcondition, arg_count__(__VA_ARGS__)) \
                                      (__VA_ARGS__)
#define postcondition1(cond)      postcondition2(cond, #cond)
#define postcondition2(cond, msg) contract_check__(postcondition, cond, msg)

#define invariant(...)            concat__(invariant, arg_count__(__VA_ARGS__)) \
                                      (__VA_ARGS__)
#define invariant1(cond)          invariant2(cond, #cond)
#define invariant2(cond, msg)     contract_check__(invariant, cond, msg)

namespace contract
{

// interface: violation handler
//

enum class type
{
    precondition,
    postcondition,
    invariant
};

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

    contract::type contract_type;
    char const * message;
    char const * condition;
    char const * file;
    std::size_t line;
};

[[noreturn]]
void handle_violation(violation_context const & context);

using violation_handler = std::function<void (violation_context const &)>;

violation_handler set_handler(violation_handler new_handler);
violation_handler get_handler();

}  // namespace contract

#include <detail/contract_imp.hpp>

// Copyright Alexei Zakharov, 2013.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#endif

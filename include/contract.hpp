#ifndef included_contract_hpp__
#define included_contract_hpp__

#include <functional>
#include <cstddef>

// interface: macros
//

#define contract(scope)           contract_ ## scope ## __

#define precondition(...)         concat__(precondition, arg_count__(__VA_ARGS__)) \
                                      (__VA_ARGS__)
#define precondition1(expr)       precondition2(expr, #expr)
#define precondition2(expr, msg)  contract_check__(precondition, expr, msg)

#define postcondition(...)        concat__(postcondition, arg_count__(__VA_ARGS__)) \
                                      (__VA_ARGS__)
#define postcondition1(expr)      postcondition2(expr, #expr)
#define postcondition2(expr, msg) contract_check__(postcondition, expr, msg)

#define invariant(...)            concat__(invariant, arg_count__(__VA_ARGS__)) \
                                      (__VA_ARGS__)
#define invariant1(expr)          invariant2(expr, #expr)
#define invariant2(expr, msg)     contract_check__(invariant, expr, msg)

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

[[noreturn]]
void handle_violation(type contr_type,
                      char const * message,
                      char const * expr,
                      char const * func,
                      char const * file,
                      std::size_t line);

using violation_handler = std::function<void (type contr_type,
                                              char const * message,
                                              char const * expr,
                                              char const * func,
                                              char const * file,
                                              std::size_t line)>;

violation_handler set_handler(violation_handler new_handler);
violation_handler get_handler();

}  // namespace contract

#include <detail/contract_imp.hpp>

#endif

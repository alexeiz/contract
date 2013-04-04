#ifndef included_contract_hpp__
#define included_contract_hpp__

#include <functional>
#include <cstddef>

// interface: macros
//

#define contract(scope)  contract_ ## scope ## __

#define precondition(expr)   contract_check__(precondition,  expr)
#define postcondition(expr)  contract_check__(postcondition, expr)
#define invariant(expr)      contract_check__(invariant,     expr)

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

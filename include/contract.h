#ifndef included_contract_h__
#define included_contract_h__

#include <cstddef>

// macros
//

#define contract_check(TYPE, MODE, EXPR, MSG)                   \
    do                                                          \
    {                                                           \
        if (contract_check_active(MODE) && !(EXPR))             \
        {                                                       \
            ::contract::failure(::contract::type:: TYPE,        \
                                ::contract::mode:: MODE,        \
                                MSG,                            \
                                #EXPR,                          \
                                __func__,                       \
                                __FILE__,                       \
                                __LINE__);                      \
        }                                                       \
    } while (0)                                                 \

#define precondition_(MODE, EXPR, MSG)  contract_check(precondition, MODE, EXPR, MSG)
#define precondition(EXPR, MSG)         precondition_(dbg, EXPR, MSG)

#define postcondition_(MODE, EXPR, MSG) contract_check(postcondition, MODE, EXPR, MSG)
#define postcondition(EXPR, MSG)        postcondition_(dbg, EXPR, MSG)

#define invariant_(MODE, EXPR, MSG)     constract_check(invariant, MODE, EXPR, MSG)
#define invariant(EXPR, MSG)            invariant_(dbg, EXPR, MSG)

// interface
//

namespace contract
{

enum class type
{
    precondition,
    invariant,
    postcondition
};

enum class mode
{
    opt,
    dbg,
    safe
};

[[noreturn]]
void failure(type contr_type,
             mode build_mode,
             char const * message,
             char const * expr,
             char const * func,
             char const * file,
             std::size_t line);

using handler = void (*)(type contr_type,
                         mode build_mode,
                         char const * message,
                         char const * expr,
                         char const * func,
                         char const * file,
                         std::size_t line);

handler set_failure(handler new_handler) noexcept;
handler get_failure() noexcept;

}  // namespace contract

#include <contract_inl.h>

#endif

#ifndef included_contract_h__
#define included_contract_h__

#define contract_check_active(MODE)  contract_check_active_ ## MODE

#define contract_check_active_opt  true

#if !defined(NDEBUG)
#   define contract_check_active_dbg  true

#   if defined(SAFE)
#       define contract_check_active_safe  false
#   endif
#endif

#if !defined(contract_check_active_opt)
#define contract_check_active_opt  false
#endif

#if !defined(contract_check_active_dbg)
#define contract_check_active_opt  false
#endif

#if !defined(contract_check_active_safe)
#define contract_check_active_opt  false
#endif

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

#define precond(MODE, EXPR)  contract_check(precondition, MODE, EXPR)
#define precond(EXPR)        precond(dbg, EXPR)

#define postcond(MODE, EXPR) contract_check(postcondition, MODE, EXPR)
#define postcond(EXPR)       postcond(dbg, EXPR)

#define invar(MODE, EXPR)    constract_check(invariant, MODE, EXPR)
#define invar(EXPR)          invar(dbg, EXPR)

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
             const char * message,
             const char * expr,
             const char * func,
             const char * file,
             std::size_t line);

using handler = void (*)(type contr_type,
                         mode build_mode,
                         const char * message,
                         const char * expr,
                         const char * func,
                         const char * file,
                         std::size_t line);

handler set_failure(handler h) noexcept;
handler get_failure() noexcept;

}

#endif

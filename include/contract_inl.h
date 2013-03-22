#ifndef included_contract_inl_h__
#define included_contract_inl_h__

#include <iostream>
#include <cstdlib>

// implementation
//

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
#define contract_check_active_dbg  false
#endif

#if !defined(contract_check_active_safe)
#define contract_check_active_safe  false
#endif

namespace contract
{

namespace detail
{

inline
void default_handler(type contr_type,
                     mode build_mode,
                     char const * message,
                     char const * expr,
                     char const * func,
                     char const * file,
                     std::size_t line)
{
    std::cerr << file << ':' << line << ": error: "
              << "contract violation of type '";

    char const * type_str;

    switch (contr_type)
    {
    case type::precondition:
        type_str = "precondition";
        break;
    case type::postcondition:
        type_str = "postcondition";
        break;
    case type::invariant:
        type_str = "invariant";
        break;
    }

    std::cerr << type_str << "'\n"
              << "message:   " << message << "\n"
              << "condition: " << expr << "\n"
              << "function:  " << func << std::endl;

    std::abort();
}

// Holder for the currently installed contract failure handler.
// Templated with a dummy type to be able to keep it in the header file.
template <typename T = void>
struct handler_holder
{
    static
    handler current_handler_;
};

template <typename T>
handler handler_holder<T>::current_handler_{default_handler};

}  // namespace detail

inline
void failure(type contr_type,
             mode build_mode,
             char const * message,
             char const * expr,
             char const * func,
             char const * file,
             std::size_t line)
{
    detail::handler_holder<>::current_handler_(contr_type,
                                               build_mode,
                                               message,
                                               expr,
                                               func,
                                               file,
                                               line);

    // if the handler returned, abort anyway to satisfy the [[noreturn]] contract
    std::abort();
}

inline
handler set_failure(handler new_handler) noexcept
{
    // TODO: make it thread-safe
    handler old_handler = detail::handler_holder<>::current_handler_;
    detail::handler_holder<>::current_handler_ = new_handler;
    return old_handler;
}

inline
handler get_failure() noexcept
{
    return detail::handler_holder<>::current_handler_;
}

}  // namespace contract

#endif

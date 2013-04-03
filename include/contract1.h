#ifndef included_contract1_h__
#define included_contract1_h__

#include <cstdlib>

#define contract(scope)  contract_ ## scope

#define contract_fun                                            \
    auto contract_obj__ = contractor()                          \
        + [&](contract_context const & contract_context__)      \

#define precondition(expr)                                      \
    do {                                                        \
        if (contract_context__.check_precondition && !(expr))   \
            std::abort();                                       \
    } while (0)                                                 \

#define postcondition(expr)                                     \
    do {                                                        \
        if (contract_context__.check_postcondition && !(expr))  \
            std::abort();                                       \
    } while (0)                                                 \

#define invariant(expr)                                         \
    do {                                                        \
        if (contract_context__.check_invariant && !(expr))      \
            std::abort();                                       \
    } while (0)                                                 \

struct contract_context
{
    bool check_precondition;
    bool check_postcondition;
    bool check_invariant;
};

template <typename ContrFunc>
struct concrete_contract
{
    concrete_contract(ContrFunc f)
        : contr_{f}
    {
        ctx_.check_precondition = true;
        ctx_.check_postcondition = false;
        ctx_.check_invariant = true;
        contr_(ctx_);
    }

    ~concrete_contract()
    {
        ctx_.check_precondition = false;
        ctx_.check_postcondition = true;
        ctx_.check_invariant = true;
        contr_(ctx_);
    }

    ContrFunc contr_;
    contract_context ctx_;
};

struct contractor
{
    template <typename Func>
    concrete_contract<Func> operator+(Func f) const
    {
        return concrete_contract<Func>{f};
    }
};

#endif

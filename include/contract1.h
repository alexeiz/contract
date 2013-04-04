#ifndef included_contract1_h__
#define included_contract1_h__

#include <type_traits>
#include <cstdlib>

#define contract(scope)  contract_ ## scope

#define contract_fun                                            \
    auto contract_obj__ = contractor<decltype(this)>(this)      \
        + [&](contract_context const & contract_context__)      \

#define contract_class                                          \
    void class_contract(                                        \
        contract_context const & contract_context__) const      \

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
struct fun_contract
{
    fun_contract(ContrFunc f)
        : contr_{f}
    {
        ctx_.check_precondition = true;
        ctx_.check_postcondition = false;
        ctx_.check_invariant = true;
        contr_(ctx_);
    }

    ~fun_contract()
    {
        ctx_.check_precondition = false;
        ctx_.check_postcondition = true;
        ctx_.check_invariant = true;
        contr_(ctx_);
    }

    ContrFunc contr_;
    contract_context ctx_;
};

// class_contract checks only invariants
template <typename ContrFunc>
struct class_contract
{
    class_contract(ContrFunc f)
        : contr_{f}
    {
        ctx_.check_precondition = false;
        ctx_.check_postcondition = false;
        ctx_.check_invariant = true;
        contr_(ctx_);
    }

    ~class_contract()
    {
        contr_(ctx_);
    }

    void operator()() const
    {
        contr_(ctx_);
    }

    ContrFunc contr_;
    contract_context ctx_;
};

template <typename T>
struct has_class_contract
{
    static auto test(int) -> decltype(declval<T>().class_contract(), std::true_type{});
};


template <typename T, bool = has_class_contract<T>::value>
struct contractor;

template <typename T>
struct contractor<T, false>
{
    contractor(T *) {}

    template <typename Func>
    concrete_contract<Func> operator+(Func f) const
    {
        return fun_contract<Func>{f};
    }
};

#endif

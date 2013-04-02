#ifndef included_contract1_h__
#define included_contract1_h__

#define contract(scope)  contract_ ## scope

#define contract_fun                                    \
    auto contract_obj_ ## __LINE__ = contractor() + [&] \

#define precondition(expr)                      \
    do {                                        \
        if (evaluate_precondition && !(expr))   \
            abort();                            \
    } while(0)                                  \

enum class contract_check
{
    precondition,
    invariant,
    postcondition,
    count__
};

bool evaluate_contract_check[contract_check::count__] = {false};

template <typename ContrFunc>
struct concrete_contract
{
    concrete_contract(ContrFunc f)
        : contr_func_{f}
    {
    }

    ~concrete_contract()
    {
    }

    ContrFunc contr_func_;
};

struct contractor
{
    template <typename Func>
    concrete_contract<Func> operator+()(Func f) const
    {
        return concrete_contract<Func>{f};
    }
};

#endif

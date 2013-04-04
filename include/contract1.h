#ifndef included_contract1_h__
#define included_contract1_h__

#include <type_traits>
#include <cstdlib>

#define contract(scope)  contract_ ## scope

#define contract_fun                                                    \
    auto contract_obj__ =                                               \
        contractor<std::remove_reference<decltype(*this)>::type>(this)  \
        + [&](contract_context const & contract_context__)              \

#define contract_class                                          \
    template <typename T>                                       \
    friend class class_contract_base;                           \
                                                                \
    template <typename T>                                       \
    friend class has_class_contract;                            \
                                                                \
    void class_contract__(                                      \
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
        contract_context ctx = {true, false, true};
        contr_(ctx);
    }

    ~fun_contract()
    {
        contract_context ctx = {false, true, true};
        contr_(ctx);
    }

    ContrFunc contr_;
};

template <typename T>
struct class_contract_base
{
    class_contract_base(T const * obj)
        : obj_(obj)
    {}

    ~class_contract_base()
    {
        contract_context ctx = {false, false, true};
        obj_->class_contract__(ctx);
    }

    T const * obj_;
};

template <typename T, typename ContrFunc>
struct class_contract : class_contract_base<T>
{
    using base_type = class_contract_base<T>;

    class_contract(T const * obj, ContrFunc f)
        : base_type{obj}
        , contr_{f}
    {
        contract_context ctx = {true, false, true};
        contr_(ctx);
    }

    ~class_contract()
    {
        contract_context ctx = {false, true, true};
        contr_(ctx);
    }

    ContrFunc contr_;
};

template <typename T>
struct has_class_contract
{
    template <typename U>
    static auto test(int) -> decltype(std::declval<U>().class_contract__(
                                          std::declval<contract_context>()),
                                      std::true_type{});
    template <typename U>
    static auto test(...) -> std::false_type;

    using type = decltype(test<T>(0));
};


template <typename T, bool = has_class_contract<T>::type::value>
struct contractor;

template <typename T>
struct contractor<T, false>
{
    contractor(T const *) {}

    template <typename Func>
    fun_contract<Func> operator+(Func f) const
    {
        return fun_contract<Func>{f};
    }
};

template <typename T>
struct contractor<T, true>
{
    contractor(T const * obj)
        : obj_(obj)
    {}

    template<typename Func>
    class_contract<T, Func> operator+(Func f) const
    {
        return class_contract<T, Func>{obj_, f};
    }

    T const * obj_;
};

#endif

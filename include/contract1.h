#ifndef included_contract1_h__
#define included_contract1_h__

#include <type_traits>
#include <cstdlib>

#define contract(scope)  contract_ ## scope

#define contract_fun                                              \
    auto contract_obj__ =                                         \
        contractor<void *>(0)                                     \
        + [&](contract_context const & contract_context__)        \

#define contract_meth                                             \
    auto contract_obj__ =                                         \
        contractor<std::remove_reference<decltype(*this)>::type>( \
            this)                                                 \
        + [&](contract_context const & contract_context__)        \

#define contract_ctor                                             \
    auto contract_obj__ =                                         \
        contractor<std::remove_reference<decltype(*this)>::type>( \
            this, false, true)                                    \
        + [&](contract_context const & contract_context__)        \

#define contract_dtor                                             \
    auto contract_obj__ =                                         \
        contractor<std::remove_reference<decltype(*this)>::type>( \
            this, true, false)                                    \
        + [&](contract_context const & contract_context__)        \

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
    contract_context(bool pre, bool post, bool inv)
        : check_precondition{pre}
        , check_postcondition{post}
        , check_invariant{inv}
    {}

    bool check_precondition;
    bool check_postcondition;
    bool check_invariant;
};

template <typename ContrFunc>
struct fun_contract
{
    explicit
    fun_contract(ContrFunc f, bool enter = true, bool exit = true)
        : contr_{f}
        , exit_{exit}
    {
        contr_(contract_context{true, false, enter});
    }

    ~fun_contract()
    {
        contr_(contract_context{false, true, exit_});
    }

    ContrFunc contr_;
    bool exit_;
};

template <typename T>
struct class_contract_base
{
    class_contract_base(T const * obj, bool enter, bool exit)
        : obj_{obj}
        , exit_{exit}
    {
        if (enter)
            obj_->class_contract__(contract_context{false, false, true});
    }

    ~class_contract_base()
    {
        if (exit_)
            obj_->class_contract__(contract_context{false, false, true});
    }

    T const * obj_;
    bool exit_;
};

template <typename T, typename ContrFunc>
struct class_contract
    : class_contract_base<T>
    , fun_contract<ContrFunc>
{
    using class_base = class_contract_base<T>;
    using fun_base = fun_contract<ContrFunc>;

    class_contract(T const * obj, ContrFunc f, bool enter, bool exit)
        : class_base{obj, enter, exit}
        , fun_base{f, enter, exit}
    {}
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
    explicit
    contractor(T const *, bool = true, bool = true) {}

    template <typename Func>
    fun_contract<Func> operator+(Func f) const
    {
        return fun_contract<Func>{f, true, true};
    }
};

template <typename T>
struct contractor<T, true>
{
    explicit
    contractor(T const * obj, bool enter = true, bool exit = true)
        : obj_{obj}
        , enter_{enter}
        , exit_{exit}
    {}

    template<typename Func>
    class_contract<T, Func> operator+(Func f) const
    {
        return class_contract<T, Func>{obj_, f, enter_, exit_};
    }

    T const * obj_;
    bool enter_;
    bool exit_;
};

#endif

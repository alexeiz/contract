struct contract_context
{
    bool precondition;
    bool postcondition;
    bool invariant;
};

template <typename T>
struct concrete_contract
{
    concrete_contract(T func)
        : func_(func)
        , context_{true, false, true}
    {
        register_context(context_);
        check_contract(func_);
    }

    ~concrete_contract()
    {
        context_ = {false, true, true};
        check_contract(func_);
    }

    T func_;
    contract_context context_;
};

struct contract_maker
{
    template <typename T>
    concrete_contract<T> operator+(T func)
    {
        return concrete_contract<T>(func);
    }
};

#define contract                                                          \
    auto contract_obj = contract_maker() + [&]{ class_contract(); } + [&] \

#define class_contract  void class_contract_method() const

class account
{
public:
    account(int deposit)
        : balance_(deposit)
    {
        contract {}
    }

    ~account()
    {
        contract {}
    }

    int withdraw(int ammount)
    {
        int withdrawn = 0;

        contract
        {
            precondition(ammount >= 0);
            postcondition(withdrawn >= 0);
        }

        withdrawn = ammount >= balance_ ? balance_ : ammount;
        balance_ -= withdrawn;
        return withdrawn;
    }

    int deposit(int ammount)
    {
        contract
        {
            precondition(ammount >= 0);
        }

        balance_ += ammount;
        return balance_;
    }

    int balance() const
    {
        contract {}
        return balance_;
    }

private:
    class_contract
    {
        invariant(balance_ >= 0);
    }

private:
    int balance_;
};

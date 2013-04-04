#include <contract1.h>

class account
{
public:
    account(int deposit)
        : balance_(-1)
    {
        contract(ctor) { precondition(deposit >= 0); };

        balance_ = deposit;
    }

    ~account()
    {
        int closing_balance = balance_;
        contract(dtor) { postcondition(closing_balance >= 0); };

        balance_ = -1;
    }

    int withdraw(int ammount)
    {
        int withdrawn = 0;

        contract(meth)
        {
            precondition(ammount >= 0);
            postcondition(withdrawn >= 0);
        };

        withdrawn = ammount >= balance_ ? balance_ : ammount;
        balance_ -= withdrawn;
        return withdrawn;
    }

    int deposit(int ammount)
    {
        contract(meth) { precondition(ammount >= 0); };

        balance_ += ammount;
        return balance_;
    }

    int balance() const
    {
        contract(meth) {};
        return balance_;
    }

private:
    contract(class) { invariant(balance_ >= 0); };

private:
    int balance_;
};

account make_account_with_fee(int balance)
{
    int const fee = 5;
    contract(fun) { precondition(balance > fee); };

    return account(balance - fee);
}

int main()
{
    account acc(100);
    acc.withdraw(10);
    acc.deposit(20);
    acc = make_account_with_fee(15);
    return acc.balance();
}

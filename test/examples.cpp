#include <contract.hpp>

#include <iostream>
#include <cstring>
#include <cassert>

// example 1: class and method contracts

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

        contract(this)
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
        contract(this) { precondition(ammount >= 0); };

        balance_ += ammount;
        return balance_;
    }

    int balance() const
    {
        contract(this) {};
        return balance_;
    }

private:
    contract(class) { invariant(balance_ >= 0, "balance can't be negative"); };

private:
    int balance_;
};

// example 2: function contracts

account make_account_with_fee(int balance)
{
    int const fee = 5;
    contract(fun) { precondition(balance > fee); };

    return account(balance - fee);
}

// example 3: function contracts

std::size_t my_strlen(char const * str)
{
    std::size_t len = 0;

    contract(fun)
    {
        precondition(str, "invalid argument");
        postcondition(len == std::strlen(str), "incorrect return value");
    };

    for (char const * p = str; *p; ++len, ++p)
        ;

    return len;
}

// example 4: custom contract violation handler

contract::violation_handler old_handler;
void my_contract_violation_handler(contract::violation_context const & context)
{
    std::cerr << "in my_contract_violation_handler:\n";
    old_handler(context);
}

int main()
{
    // example 4
    old_handler = contract::set_handler(my_contract_violation_handler);

    // example 3
    my_strlen("a string");

    // example 1
    account acc(100);
    acc.withdraw(10);
    acc.deposit(20);
    assert(acc.balance() == 110);

    // example 2
    acc = make_account_with_fee(15);
}

// Copyright Alexei Zakharov, 2013.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

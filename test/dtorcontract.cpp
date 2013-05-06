#include <contract.hpp>

#include "contract_error.hpp"

namespace
{

class account
{
public:
    account(int bal)
        : balance_(bal)
    {}

    ~account() noexcept(false)
    {
        contract(dtor)
        {
            precondition(balance_ > 0);
            postcondition(balance_ < 0);
        };

        balance_ -= 100;
    }

private:
    int balance_;
};

}

BOOST_AUTO_TEST_CASE(dtor_contract)
{
    test::contract_handler_frame cframe;

    BOOST_CHECK_NO_THROW(account(10));

    test::check_throw_on_contract_violation([] { account(0); },
                                            contract::type::precondition);
    test::check_throw_on_contract_violation([] { account(200); },
                                            contract::type::postcondition);
}

// Copyright Alexei Zakharov, 2013.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

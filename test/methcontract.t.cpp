#include <contract/contract.hpp>

#include "contract_error.hpp"

namespace
{
class account
{
public:
    account(int bal)
        : balance_{bal}
    {}

    void balance(int bal)
    {
        int old_balance = balance_;

        contract(this)
        {
            precondition(bal >= 0);
            invariant(balance_ > 0);
            postcondition(balance_ >= old_balance);
        };

        balance_ = bal;
    }

private:
    int balance_;
};
}  // anonymous namespace

#include <contract/undef.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(method_contract)
{
    test::contract_handler_frame cframe;

    // expect contract to pass
    BOOST_CHECK_NO_THROW(account(10).balance(20));

    // expect precondition to fail
    test::check_throw_on_contract_violation([]{ account(10).balance(-1); },
                                            contract::type::precondition);

    // expect postcondition to fail
    test::check_throw_on_contract_violation([]{ account(10).balance(5); },
                                            contract::type::postcondition);

    // expect invariant to fail
    test::check_throw_on_contract_violation([]{ account(-10).balance(5); },
                                            contract::type::invariant);
    test::check_throw_on_contract_violation([]{ account(0).balance(0); },
                                            contract::type::invariant);
}

// Copyright Alexei Zakharov, 2025.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

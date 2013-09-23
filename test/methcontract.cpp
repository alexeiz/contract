#include <contract.hpp>

#include "contract_error.hpp"

#include <boost/test/unit_test.hpp>

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

}

BOOST_AUTO_TEST_CASE(method_contract)
{
    test::contract_handler_frame cframe;

    // satisfy contract
    BOOST_CHECK_NO_THROW(account(10).balance(20));

    // violate precondition
    test::check_throw_on_contract_violation([]{ account(10).balance(-1); },
                                            contract::type::precondition);

    // violate postcondition
    test::check_throw_on_contract_violation([]{ account(10).balance(5); },
                                            contract::type::postcondition);

    // violate invariant
    test::check_throw_on_contract_violation([]{ account(-10).balance(5); },
                                            contract::type::invariant);
    test::check_throw_on_contract_violation([]{ account(0).balance(0); },
                                            contract::type::invariant);
}

// Copyright Alexei Zakharov, 2013.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

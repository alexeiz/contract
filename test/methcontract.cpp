#include <contract.hpp>

#include "contract_error.hpp"

#define BOOST_TEST_MODULE methcontract
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

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

template <typename Func>
void check_throw_on_contract_violation(Func f, contract::type type)
{
    bool caught_exception = false;

    try
    {
        f();
    }
    catch (test::contract_error & e)
    {
        caught_exception = true;
        BOOST_CHECK(e.type() == type);
    }
    catch (...)
    {
        caught_exception = true;
        BOOST_FAIL("expected to catch test::contract_error");
    }

    BOOST_CHECK(caught_exception);
}

BOOST_AUTO_TEST_CASE(method_contract)
{
    test::contract_handler_frame cframe;

    // satisfy contract
    BOOST_CHECK_NO_THROW(account(10).balance(20));

    // violate precondition
    check_throw_on_contract_violation([]{ account(10).balance(-1); },
                                      contract::type::precondition);

    // violate postcondition
    check_throw_on_contract_violation([]{ account(10).balance(5); },
                                      contract::type::postcondition);

    // violate invariant
    check_throw_on_contract_violation([]{ account(-10).balance(5); },
                                      contract::type::invariant);
    check_throw_on_contract_violation([]{ account(0).balance(0); },
                                      contract::type::invariant);
}

// Copyright Alexei Zakharov, 2013.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

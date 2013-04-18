#include <contract.hpp>

#include "contract_error.hpp"

#define BOOST_TEST_MODULE dtorcontract
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

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

BOOST_AUTO_TEST_CASE(dtor_contract)
{
    test::contract_handler_frame cframe;

    BOOST_CHECK_NO_THROW(account(10));

    check_throw_on_contract_violation([] { account(0); },
                                      contract::type::precondition);
    check_throw_on_contract_violation([] { account(200); },
                                      contract::type::postcondition);
}

// Copyright Alexei Zakharov, 2013.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

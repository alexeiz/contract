#include <contract/contract.hpp>

#include "contract_error.hpp"

namespace
{
class throwing_ctor_t {};

class account
{
public:
    account(int bal)
        : balance_(-1)
    {
        contract(ctor) {};
        balance_ = bal;  // the class contract is checked on constructor exit
    }

    account(throwing_ctor_t)
        : balance_(-1)
    {
        contract(ctor) {};
        throw test::non_contract_error{};
    }

    ~account() noexcept(false)  // only needed so the contract check can throw
                                // an exception; if the contract check aborts
                                // the destructor can and should be noexcept
    {
        contract(dtor) {};
        balance_ = -1;  // the class contract is checked on destructor entry
    }

    int balance() const
    {
        contract(this) {};
        return balance_;
    }

    void balance(int bal)
    {
        contract(this) {};
        balance_ = bal;  // the class contract is checked both on method
                         // entry and exit
    }

private:
    contract(class) { invariant(balance_ > 0, "invariant"); };

private:
    int balance_;
};
}  // anonymous namespace

#include <contract/undef.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(class_contract_in_ctor_dtor)
{
    test::contract_handler_frame cframe;

    // expect class invariant to pass
    BOOST_CHECK_NO_THROW(account(10));

    // expect class invariant to fail
    BOOST_CHECK_THROW(account(-2), test::contract_error);

    // skip class invariant if constructor throws
    BOOST_CHECK_THROW(account(throwing_ctor_t{}), test::non_contract_error);
}

BOOST_AUTO_TEST_CASE(class_contract_in_method)
{
    test::contract_handler_frame cframe;

    bool caught_exception = false;

    try
    {
        // expect class invariant to pass
        account acc(10);
        BOOST_CHECK_NO_THROW(acc.balance());
        BOOST_CHECK_NO_THROW(acc.balance(20));

        // once the invariant is violated, all methods start to throw
        // the contract error
        BOOST_CHECK_THROW(acc.balance(-2), test::contract_error);
        BOOST_CHECK_THROW(acc.balance(), test::contract_error);

        // verify that the contract is checked on both entry/exit of a method,
        // and the class invariant can't be restored once it's broken
        BOOST_CHECK_THROW(acc.balance(15), test::contract_error);
        BOOST_CHECK_THROW(acc.balance(), test::contract_error);

        // verify that the destructor also throws on contract violation
    }
    catch (test::contract_error & e)
    {
        caught_exception = true;
        BOOST_CHECK(e.type() == contract::type::invariant);
    }
    catch (...)
    {
        caught_exception = true;
        BOOST_FAIL("expected to catch test::contract_error");
    }

    BOOST_CHECK(caught_exception);
}

// Copyright Alexei Zakharov, 2025.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

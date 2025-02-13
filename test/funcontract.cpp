#include <contract/contract.hpp>

#include "contract_error.hpp"

namespace
{
void fun_contract_test_precondition(bool par)
{
    contract(fun) { precondition(par); };
}

void fun_contract_test_precondition(bool par, char const * msg)
{
    contract(fun) { precondition(par, msg); };
}

void fun_contract_test_invariant(bool par)
{
    contract(fun) { invariant(par); };
}

void fun_contract_test_invariant(bool par, char const * msg)
{
    contract(fun) { invariant(par, msg); };
}

void fun_contract_test_postcondition(bool par)
{
    contract(fun) { postcondition(par); };
}

void fun_contract_test_postcondition(bool par, char const * msg)
{
    contract(fun) { postcondition(par, msg); };
}

void fun_contract_test_postcondition_exception()
{
    contract(fun) { postcondition(false); };
    throw test::non_contract_error{};
}

void fun_contract_test_all(bool pre, bool inv, bool post)
{
    contract(fun)
    {
        precondition(pre);
        invariant(inv);
        postcondition(post);
    };
}
}  // anonymous namespace

#include <contract/undef.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(fun_contract_precondition)
{
    test::contract_handler_frame cframe;

    // expect precondition to pass
    BOOST_CHECK_NO_THROW(fun_contract_test_precondition(true));
    BOOST_CHECK_NO_THROW(fun_contract_test_precondition(true, "message"));

    // expect precondition to fail
    test::check_throw_on_contract_violation(
        []{ fun_contract_test_precondition(false); },
        contract::type::precondition);

    test::check_throw_on_contract_violation(
        []{ fun_contract_test_precondition(false, "precondition"); },
        contract::type::precondition,
        "precondition");

    test::check_throw_on_contract_violation(
        []{ fun_contract_test_all(false, true, true); },
        contract::type::precondition);
}

BOOST_AUTO_TEST_CASE(fun_contract_invariant)
{
    test::contract_handler_frame cframe;

    // expect invariant to pass
    BOOST_CHECK_NO_THROW(fun_contract_test_invariant(true));
    BOOST_CHECK_NO_THROW(fun_contract_test_invariant(true, "message"));

    // expect invariant to fail
    test::check_throw_on_contract_violation(
        []{ fun_contract_test_invariant(false); },
        contract::type::invariant);

    test::check_throw_on_contract_violation(
        []{ fun_contract_test_invariant(false, "invariant"); },
        contract::type::invariant,
        "invariant");

    test::check_throw_on_contract_violation(
        []{ fun_contract_test_all(true, false, true); },
        contract::type::invariant);
}

BOOST_AUTO_TEST_CASE(fun_contract_postcondition)
{
    test::contract_handler_frame cframe;

    // expect postcondition to pass
    BOOST_CHECK_NO_THROW(fun_contract_test_postcondition(true));
    BOOST_CHECK_NO_THROW(fun_contract_test_postcondition(true, "message"));

    // expect postcondition to fail
    test::check_throw_on_contract_violation(
        []{ fun_contract_test_postcondition(false); },
        contract::type::postcondition);

    test::check_throw_on_contract_violation(
        []{ fun_contract_test_postcondition(false, "postcondition"); },
        contract::type::postcondition,
        "postcondition");

    test::check_throw_on_contract_violation(
        []{ fun_contract_test_all(true, true, false); },
        contract::type::postcondition);

    // skip postcondition if function throws
    BOOST_CHECK_THROW(fun_contract_test_postcondition_exception(),
                      test::non_contract_error);
}

BOOST_AUTO_TEST_CASE(fun_contract_all)
{
    // expect contract to pass
    BOOST_CHECK_NO_THROW(fun_contract_test_all(true, true, true));
}

// Copyright Alexei Zakharov, 2013.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

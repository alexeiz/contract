#include <contract.hpp>

#include "contract_error.hpp"

#define BOOST_TEST_MODULE funcontract
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

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

void fun_contract_test_all(bool pre, bool inv, bool post)
{
    contract(fun)
    {
        precondition(pre);
        invariant(inv);
        postcondition(post);
    };
}

template <typename Func>
void check_throw_on_contract_violation(Func f, contract::type type)
{
    try
    {
        f();
    }
    catch (test::contract_error & e)
    {
        BOOST_CHECK(e.type() == type);
    }
    catch (...)
    {
        BOOST_FAIL("expected to catch test::contract_error");
    }
}

template <typename Func>
void check_throw_on_contract_violation(Func f, contract::type type, char const * msg)
{
    try
    {
        f();
    }
    catch (test::contract_error & e)
    {
        BOOST_CHECK(e.type() == type);
        BOOST_CHECK(e.message() == std::string(msg));
    }
    catch (...)
    {
        BOOST_FAIL("expected to catch test::contract_error");
    }
}

BOOST_AUTO_TEST_CASE(fun_contract_precondition)
{
    test::contract_handler_frame cframe;

    check_throw_on_contract_violation(
        []{ fun_contract_test_precondition(false); },
        contract::type::precondition);

    check_throw_on_contract_violation(
        []{ fun_contract_test_precondition(false, "precondition"); },
        contract::type::precondition,
        "precondition");

    check_throw_on_contract_violation(
        []{ fun_contract_test_all(false, true, true); },
        contract::type::precondition);

    BOOST_CHECK_NO_THROW(fun_contract_test_precondition(true));
}

BOOST_AUTO_TEST_CASE(fun_contract_invariant)
{
    test::contract_handler_frame cframe;

    check_throw_on_contract_violation(
        []{ fun_contract_test_invariant(false); },
        contract::type::invariant);

    check_throw_on_contract_violation(
        []{ fun_contract_test_invariant(false, "invariant"); },
        contract::type::invariant,
        "invariant");

    check_throw_on_contract_violation(
        []{ fun_contract_test_all(true, false, true); },
        contract::type::invariant);

    BOOST_CHECK_NO_THROW(fun_contract_test_invariant(true));
}

BOOST_AUTO_TEST_CASE(fun_contract_postcondition)
{
    test::contract_handler_frame cframe;

    check_throw_on_contract_violation(
        []{ fun_contract_test_postcondition(false); },
        contract::type::postcondition);

    check_throw_on_contract_violation(
        []{ fun_contract_test_postcondition(false, "postcondition"); },
        contract::type::postcondition,
        "postcondition");

    check_throw_on_contract_violation(
        []{ fun_contract_test_all(true, true, false); },
        contract::type::postcondition);

    BOOST_CHECK_NO_THROW(fun_contract_test_postcondition(true));
}

BOOST_AUTO_TEST_CASE(fun_contract_all)
{
    BOOST_CHECK_NO_THROW(fun_contract_test_all(true, true, true));
}

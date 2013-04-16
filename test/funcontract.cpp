#include <contract.hpp>

#include "contract_error.hpp"

#define BOOST_TEST_MODULE funcontract
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

bool fun_contract_test_precondition(bool par)
{
    contract(fun) { precondition(par); };
    return par;
}

bool fun_contract_test_invariant(bool par)
{
    contract(fun) { invariant(par); };
    return par;
}

bool fun_contract_test_postcondition(bool par)
{
    contract(fun) { postcondition(par); };
    return par;
}

BOOST_AUTO_TEST_CASE(fun_contract_precondition)
{
    test::contract_handler_frame cframe;
    BOOST_CHECK_THROW(fun_contract_test_precondition(false), test::contract_error);
    BOOST_CHECK_NO_THROW(fun_contract_test_precondition(true));
}

BOOST_AUTO_TEST_CASE(fun_contract_invariant)
{
    test::contract_handler_frame cframe;
    BOOST_CHECK_THROW(fun_contract_test_invariant(false), test::contract_error);
    BOOST_CHECK_NO_THROW(fun_contract_test_invariant(true));
}

BOOST_AUTO_TEST_CASE(fun_contract_postcondition)
{
    test::contract_handler_frame cframe;
    BOOST_CHECK_THROW(fun_contract_test_postcondition(false), test::contract_error);
    BOOST_CHECK_NO_THROW(fun_contract_test_postcondition(true));
}

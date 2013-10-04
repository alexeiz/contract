#define CONTRACT_DISABLE_PRECONDITIONS
#include <contract/contract.hpp>

#include "contract_error.hpp"

#include <boost/test/unit_test.hpp>

namespace
{

void test_disable_preconditions()
{
    contract (fun) { precondition(false); };
}

void test_disable_preconditions_postcondition()
{
    contract (fun) { postcondition(false); };
}

void test_disable_preconditions_invariant()
{
    contract (fun) { invariant(false); };
}

}

BOOST_AUTO_TEST_CASE(macro_disable_preconditions)
{
    test::contract_handler_frame cframe;

    // expect disabled precondition
    BOOST_CHECK_NO_THROW(test_disable_preconditions());

    // expect postcondition to fail
    BOOST_CHECK_THROW(test_disable_preconditions_postcondition(),
                      test::contract_error);

    // expect invariant to fail
    BOOST_CHECK_THROW(test_disable_preconditions_invariant(),
                      test::contract_error);
}

// Copyright Alexei Zakharov, 2013.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

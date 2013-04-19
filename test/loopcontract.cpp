#include <contract.hpp>

#include "contract_error.hpp"

#define BOOST_TEST_MODULE loopcontract
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

void loop_invariant_success()
{
    for (int i = 0; i != 10; ++i)
    {
        contract(loop)
        {
            invariant(i >= 0);
            invariant(i < 10);
        };
    }
}

void loop_invariant_failure()
{
    for (int i = 0; i != 10; ++i)
    {
        contract(loop)
        {
            invariant((i & 1) == 0);
        };
    }
}

void ignore_pre_postcondition()
{
    for (int i = 0; i != 10; ++i)
    {
        contract(loop)
        {
            precondition((i & 1) == 0);
            postcondition((i & 1) != 0);
        };
    }
}

BOOST_AUTO_TEST_CASE(loop_invariant)
{
    test::contract_handler_frame cframe;

    BOOST_CHECK_NO_THROW(loop_invariant_success());
    BOOST_CHECK_THROW(loop_invariant_failure(), test::contract_error);

    BOOST_CHECK_NO_THROW(ignore_pre_postcondition());
}

// Copyright Alexei Zakharov, 2013.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <contract/contract.hpp>

#include "contract_error.hpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(get_default_handler)
{
    contract::violation_handler default_handler{contract::get_handler()};
    BOOST_CHECK(default_handler);
}

BOOST_AUTO_TEST_CASE(set_violation_handler)
{
    contract::set_handler(test::throw_contract_error);

    bool caught_exception = false;

    try
    {
        contract::handle_violation(
            contract::violation_context{contract::type::postcondition,
                                        "1 message",
                                        "2 expr",
                                        "3 file",
                                        4});
    }
    catch (test::contract_error & e)
    {
        caught_exception = true;
        BOOST_CHECK(e.type() == contract::type::postcondition);
        BOOST_CHECK_EQUAL(e.message(), "1 message");
        BOOST_CHECK_EQUAL(e.condition(), "2 expr");
        BOOST_CHECK_EQUAL(e.file(), "3 file");
        BOOST_CHECK_EQUAL(e.line(), 4);
    }
    catch (...)
    {
        caught_exception = true;
        BOOST_FAIL("expected to catch test::contract_error");
    }

    BOOST_CHECK(caught_exception);
}

// Copyright Alexei Zakharov, 2013.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

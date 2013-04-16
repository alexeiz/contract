#include <contract.hpp>

#define BOOST_TEST_MODULE funcontract
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(simple_contract)
{
    BOOST_CHECK_EQUAL(2 + 2, 5);
}

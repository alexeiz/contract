#include <contract.hpp>

#include "contract_error.hpp"

#define BOOST_TEST_MODULE ctorcontract
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

class account
{
public:
    account(int bal, bool skip_pre = false)
        : balance_(-1)
    {
        contract(ctor)
        {
            precondition(bal > 0 || skip_pre);
            postcondition(balance_ > 0);
        };

        balance_ = bal;  // after balance_ is assigned the postcondition succeeds
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

BOOST_AUTO_TEST_CASE(ctor_contract)
{
    test::contract_handler_frame cframe;

    BOOST_CHECK_NO_THROW(account(10));

    check_throw_on_contract_violation([] { account(0); },
                                      contract::type::precondition);
    check_throw_on_contract_violation([] { account(0, true); },
                                      contract::type::postcondition);
}

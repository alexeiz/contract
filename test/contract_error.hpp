#ifndef included_contract_error_hpp__
#define included_contract_error_hpp__

#include <contract/contract.hpp>

#include <cassert>
#include <exception>
#include <stdexcept>
#include <sstream>
#include <string>

namespace test
{
struct non_contract_error {};

struct contract_error : std::exception
{
    contract_error(contract::violation_context const & context)
        : context_{context}
    {
        std::ostringstream err;
        err << context.file << ':' << context.line
            << ": error: contract violation of type '";

        char const * type_str;

        switch (context.contract_type)
        {
        case contract::type::precondition:
            type_str = "precondition";
            break;
        case contract::type::postcondition:
            type_str = "postcondition";
            break;
        case contract::type::invariant:
            type_str = "invariant";
            break;
        }

        err << type_str << "'\n"
            << "message:   " << context.message << "\n"
            << "condition: " << context.condition << std::endl;

        error_ = err.str();
    }

    char const * what() const noexcept override
    {
        return error_.c_str();
    }

    contract::type type() const { return context_.contract_type; }
    char const * message() const { return context_.message; }
    char const * condition() const { return context_.condition; }
    char const * file() const { return context_.file; }
    std::size_t line() const { return context_.line; }

private:
    contract::violation_context context_;
    std::string error_;
};

inline
void throw_contract_error(contract::violation_context const & context)
{
    throw contract_error(context);
}

template <typename = void>
struct terminate_holder
{
    static
    std::terminate_handler default_terminate;
};

template <typename T>
std::terminate_handler terminate_holder<T>::default_terminate{nullptr};

inline
void terminate()
{
    terminate_holder<>::default_terminate();
}

struct contract_handler_frame
{
    contract_handler_frame()
        : old_handler_{contract::set_handler(throw_contract_error)}
    {
        terminate_holder<>::default_terminate = std::set_terminate(terminate);
    }

    ~contract_handler_frame()
    {
        contract::set_handler(old_handler_);
        std::set_terminate(terminate_holder<>::default_terminate);
    }

    contract::violation_handler old_handler_;
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
        assert(e.type() == type);
    }
    catch (...)
    {
        caught_exception = true;
        assert(0 && "expected to catch test::contract_error");
    }

    assert(caught_exception);
}

template <typename Func>
void check_throw_on_contract_violation(Func f, contract::type type, char const * msg)
{
    bool caught_exception = false;

    try
    {
        f();
    }
    catch (test::contract_error & e)
    {
        caught_exception = true;
        assert(e.type() == type);
        assert(e.message() == std::string(msg));
    }
    catch (...)
    {
        caught_exception = true;
        assert("expected to catch test::contract_error");
    }

    assert(caught_exception);
}
}  // namespace test

// Copyright Alexei Zakharov, 2025.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#endif

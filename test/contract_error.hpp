#ifndef included_contract_error_hpp__
#define included_contract_error_hpp__

#include <contract.hpp>

#include <exception>
#include <stdexcept>
#include <sstream>
#include <string>

namespace test
{

struct contract_error : std::exception
{
    contract_error(contract::type contr_type,
                   char const * message,
                   char const * expr,
                   char const * func,
                   char const * file,
                   std::size_t line)
        : type_(contr_type)
        , message_(message)
        , expr_(expr)
        , func_(func)
        , file_(file)
        , line_(line)
    {
        std::ostringstream err;
        err << file << ':' << line << ": error: "
            << "contract violation of type '";

        char const * type_str;

        switch (contr_type)
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
            << "message:   " << message << "\n"
            << "condition: " << expr << "\n"
            << "function:  " << func << std::endl;

        error_ = err.str();
    }

    char const * what() const noexcept override
    {
        return error_.c_str();
    }

    contract::type type() const { return type_; }
    char const * message() const { return message_; }
    char const * expr() const { return expr_; }
    char const * func() const { return func_; }
    char const * file() const { return file_; }
    std::size_t line() const { return line_; }

private:
    contract::type type_;
    char const * message_;
    char const * expr_;
    char const * func_;
    char const * file_;
    std::size_t line_;
    std::string error_;
};

inline
void throw_contract_error(contract::type contr_type,
                          char const * message,
                          char const * expr,
                          char const * func,
                          char const * file,
                          std::size_t line)
{
    throw contract_error(contr_type, message, expr, func, file, line);
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

}

// Copyright Alexei Zakharov, 2013.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#endif

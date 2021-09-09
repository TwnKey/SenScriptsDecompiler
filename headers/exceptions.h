#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <exception>

struct exception_past_the_end_addr : public std::exception
{
    const char * what () const throw ()
    {
        return "ERROR: index reached the next function.";
    }
};

struct exception_incorrect_OP_code : public std::exception
{
    const char * what () const throw ()
    {
        return "ERROR: OP code is not defined.";
    }
};

struct exception_unexpected_operand : public std::exception
{
    const char * what () const throw ()
    {
        return "ERROR: unexpected operand.";
    }
};
#endif // EXCEPTIONS_H

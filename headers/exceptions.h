#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <stdexcept>

class exception_past_the_end_addr : public std::runtime_error {
  public:
    explicit exception_past_the_end_addr(const std::string& msg = "")
      : std::runtime_error("ERROR: index reached the next function: " + msg) {}
};

class exception_incorrect_OP_code : public std::runtime_error {
  public:
    explicit exception_incorrect_OP_code(const std::string& msg = "")
      : std::runtime_error("ERROR: OP code is not defined:" + msg) {}
};

class exception_unexpected_operand : public std::runtime_error {
  public:
    explicit exception_unexpected_operand(const std::string& msg = "")
      : std::runtime_error("ERROR: unexpected operand" + msg) {}
};
#endif // EXCEPTIONS_H

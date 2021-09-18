#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <stdexcept>

namespace ssd::exceptions {
class recoverable : public std::runtime_error {
  public:
    explicit recoverable(const std::string& msg = "")
      : std::runtime_error(msg) {}
};

class unspecified_recoverable : public recoverable {
  public:
    explicit unspecified_recoverable(const std::string& msg = "")
      : recoverable("ERROR: unspecified parser problem: " + msg) {}
};

class past_the_end_addr : public recoverable {
  public:
    explicit past_the_end_addr(const std::string& msg = "")
      : recoverable("ERROR: index reached the next function: " + msg) {}
};

class bad_opcode : public recoverable {
  public:
    explicit bad_opcode(const std::string& msg = "")
      : recoverable("ERROR: OP code is not defined:" + msg) {}
};

class unexpected_operand : public recoverable {
  public:
    explicit unexpected_operand(const std::string& msg = "")
      : recoverable("ERROR: unexpected operand:" + msg) {}
};
} // namespace ssd::exceptions
#endif // EXCEPTIONS_H

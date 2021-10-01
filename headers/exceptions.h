#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <stdexcept>

#include "fmt/compile.h"
#include "fmt/format.h"

namespace ssd::exceptions {
class recoverable : public std::runtime_error {
  public:
    explicit recoverable(const std::string& msg = "")
      : std::runtime_error(msg) {}
};

class unspecified_recoverable : public recoverable {
  public:
    explicit unspecified_recoverable(const std::string& msg = "")
      : recoverable(fmt::format(FMT_COMPILE("ERROR: unspecified parser problem: {}"), msg)) {}
};

class past_the_end_addr : public recoverable {
  public:
    explicit past_the_end_addr(const std::string& msg = "")
      : recoverable(fmt::format(FMT_COMPILE("ERROR: index reached the next function: {}"), msg)) {}
};

class bad_opcode : public recoverable {
  public:
    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    explicit bad_opcode(int opcode, int address)
      : recoverable(fmt::format(FMT_COMPILE("ERROR: Opcode {:#04X} is not defined for address {:#04X}"), opcode, address)) {}
};

class unexpected_operand : public recoverable {
  public:
    explicit unexpected_operand(const std::string& msg = "")
      : recoverable(fmt::format(FMT_COMPILE("ERROR: unexpected operand: {}"), msg)) {}
};
} // namespace ssd::exceptions
#endif // EXCEPTIONS_H

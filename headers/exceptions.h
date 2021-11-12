#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <stdexcept>

#include "fmt/compile.h"
#include "fmt/format.h"

namespace ssd::exceptions {

class parse_error : public std::runtime_error {
  public:
    explicit parse_error(const std::string& msg = "")
      : std::runtime_error(msg) {}
};

class not_analyzed_yet : public parse_error {
  public:
    explicit not_analyzed_yet(std::integral auto byte)
      : parse_error(fmt::format(FMT_COMPILE("ERROR: byte {:#04x} is not analzyed yet"), byte)) {}
};

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
      : recoverable(fmt::format(FMT_COMPILE("ERROR: Opcode {:#04x} is not defined for address {:#04x}"), opcode, address)) {}
};

class unexpected_operand : public recoverable {
  public:
    explicit unexpected_operand(const std::string& msg = "")
      : recoverable(fmt::format(FMT_COMPILE("ERROR: unexpected operand: {}"), msg)) {}
};
} // namespace ssd::exceptions
#endif // EXCEPTIONS_H

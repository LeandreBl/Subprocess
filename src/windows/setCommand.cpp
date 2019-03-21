#ifdef _WIN32
#include <cstring>

#include "Process.hpp"

namespace lp {
  void Process::setCommand(const std::string &command) noexcept
  {
    _cmd = command;
  }
}
#endif
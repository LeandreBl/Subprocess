#include "Process.hpp"


namespace lp {
  const std::string &Process::getCommand() const noexcept
  {
    return _cmd;
  }
}

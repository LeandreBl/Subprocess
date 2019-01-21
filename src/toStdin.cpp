#include <unistd.h>

#include "Process.hpp"

namespace lp {
  void Process::toStdin(const std::string &string) noexcept
  {
    write(_pipes[stdin][1], string.c_str(), string.size());
  }
} // namespace lp

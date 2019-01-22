#include <unistd.h>

#include "Process.hpp"

namespace lp {
  ssize_t Process::writeStdin(const std::string &string) noexcept
  {
    if (isRedirecting(Stdin) == false)
      return -1;
    return write(_pipes[Stdin][1], string.c_str(), string.size());
  }
} // namespace lp

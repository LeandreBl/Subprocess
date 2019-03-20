#include <unistd.h>
#include <cstring>

#include "Process.hpp"

namespace lp {
  ssize_t Process::writeStdin(const std::string &string) const noexcept
  {
    return writeStdin(string.c_str(), string.size());
  }
#ifdef _WIN32
  ssize_t Process::writeStdin(const void *data, size_t count) const noexcept
  {
    if (isRedirecting(Stdin) == false)
      return -1;
    return write(_pipes[Stdin][0], data, count);
  }
#endif
} // namespace lp

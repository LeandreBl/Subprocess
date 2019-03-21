#ifndef _WIN32
# include <unistd.h>
#endif
#include <cstring>

#include "Process.hpp"

namespace lp {
  ssize_t Process::writeStdin(const std::string &string) const noexcept
  {
    return writeStdin(string.c_str(), string.size());
  }
  ssize_t Process::writeStdin(const void *data, size_t count) const noexcept
  {
    if (isRedirecting(Stdin) == false)
      return -1;
#ifdef _WIN32
    ssize_t wr;
    if (!WriteFile(_pipes[Stdin][0], data, (DWORD)count, (LPDWORD)&wr, NULL))
      return -1;
    return wr;
#else
    return write(_pipes[Stdin][0], data, count);
#endif
  }
} // namespace lp

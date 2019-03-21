#ifdef _WIN32
#include <cstring>

#include "Process.hpp"

namespace lp {
  ssize_t Process::writeStdin(const void *data, size_t count) const noexcept
  {
    if (isRedirecting(Stdin) == false)
      return -1;
    ssize_t wr;
    if (!WriteFile(_pipes[Stdin][0], data, (DWORD)count, (LPDWORD)&wr, NULL))
      return -1;
    return wr;
  }
}
#endif
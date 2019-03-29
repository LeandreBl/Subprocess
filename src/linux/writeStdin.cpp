#ifdef __linux__
#  include <unistd.h>
#  include <cstring>

#  include "Process.hpp"

namespace lp {
int64_t Process::writeStdin(const void *data, size_t count) const noexcept
{
  if (isRedirecting(Stdin) == false)
    return -1;
  return write(_pipes[Stdin][0], data, count);
}
} // namespace lp
#endif
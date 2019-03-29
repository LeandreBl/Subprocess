#include <cstring>

#include "Process.hpp"

namespace lp {
int64_t Process::writeStdin(const std::string &string) const noexcept
{
  return writeStdin(string.c_str(), string.size());
}
} // namespace lp

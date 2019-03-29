#include <assert.h>

#include "Process.hpp"

namespace lp {
std::stringstream &Process::getStdout() noexcept
{
  return getStream(Stdout);
}
std::stringstream &Process::getStderr() noexcept
{
  return getStream(Stderr);
}
std::stringstream &Process::getStream(enum streamType stream) noexcept
{
  assert(stream != Stdin);
  return _streams[stream - 1];
}
} // namespace lp
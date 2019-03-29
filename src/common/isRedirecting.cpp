#include "Process.hpp"

namespace lp {
bool Process::isRedirecting(enum streamType stream) const noexcept
{
  return (_redirect & (0b100 >> stream));
}
} // namespace lp
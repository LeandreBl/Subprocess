#include "Process.hpp"

namespace lp {
  void Process::redirect(enum streamType stream, bool does_redirect) noexcept
  {
    if (does_redirect)
      _redirect |= (0b100 >> stream);
    else
      _redirect ^= (0b100 >> stream);
  }
} // namespace lp
#include "Process.hpp"

namespace lp {
  void Process::redirectStdin(bool does_redirect) noexcept
  {
    redirectStream(Stdin, does_redirect);
  }

  void Process::redirectStdout(bool does_redirect) noexcept
  {
    redirectStream(Stdout, does_redirect);
  }

  void Process::redirectStderr(bool does_redirect) noexcept
  {
    redirectStream(Stderr, does_redirect);
  }

  void Process::redirectStream(enum streamType stream, bool does_redirect) noexcept
  {
    if (does_redirect == true)
      _redirect |= (0b100 >> stream);
    else
      _redirect ^= (0b100 >> stream);
  }
}
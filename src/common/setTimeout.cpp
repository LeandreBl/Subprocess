#include "Process.hpp"

namespace lp {
  void Process::setStreamTimeout(int timeout) noexcept
  {
    _pollTimeout = timeout;
  }
}
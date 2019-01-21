#include "Process.hpp"

namespace lp {
  void Process::onReceive(enum streamType stream, const std::function<void(Process &process, std::istringstream &stream)> &callback) noexcept
  {
    if (stream != stdin)
      _callbacks[stream - 1] = callback;
  }
}
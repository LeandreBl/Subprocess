#include "Process.hpp"

namespace lp {
  void Process::onReadStdout(const std::function<void (Process &process, std::stringstream &stream)> &callback) noexcept
  {
    onRead(Stdout, callback);
  }
  void Process::onReadStderr(const std::function<void (Process &process, std::stringstream &stream)> &callback) noexcept
  {
    onRead(Stderr, callback);
  }
  void Process::onRead(enum streamType stream, const std::function<void(Process &process, std::stringstream &stream)> &callback) noexcept
  {
    if (stream != Stdin)
      _callbacks[stream - 1] = callback;
  }
}
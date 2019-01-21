#include "Process.hpp"

namespace lp {
  bool Process::isRunning() noexcept
  {
    if (_pid == -1) {
      _isRunning = false;
      return false;
    }
    pid_t pid = waitpid(_pid, &_status, WNOHANG);
    _isRunning = (pid == 0 || pid == _pid);
    if (_isRunning == false)
      _pid = -1;
    return _isRunning;
  }
}
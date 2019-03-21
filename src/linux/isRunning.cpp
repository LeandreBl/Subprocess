#ifdef __linux__
#include "Process.hpp"

namespace lp {
  bool Process::isRunning() noexcept
  {
    if (_pid == -1) {
      _isRunning = false;
      return false;
    }
    int status;
    pid_t pid = waitpid(_pid, &status, WNOHANG);
    _isRunning = (pid == 0 || pid == _pid);
    if (_isRunning == false) {
      _status = WEXITSTATUS(status);
      _pid = -1;
    }
    return _isRunning;
  }
}
#endif

#include "Process.hpp"

namespace lp {
  bool Process::isRunning() noexcept
  {
#ifdef _WIN32
    if (_isRunning == false)
      return false;
    DWORD status = WaitForSingleObject(_pi.hProcess, 1);
    _isRunning = !(status == WAIT_OBJECT_0);
    if (_isRunning == false) {
      GetExitCodeProcess(_pi.hProcess, &_status);
      CloseHandle(_pi.hProcess);
      CloseHandle(_pi.hThread);
    }
    return _isRunning;
#else
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
#endif
  }
}

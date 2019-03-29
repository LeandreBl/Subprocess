#ifdef _WIN32
#  include "Process.hpp"

namespace lp {
bool Process::isRunning() noexcept
{
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
}
} // namespace lp
#endif
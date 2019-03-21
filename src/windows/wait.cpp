#ifdef _WIN32

#include <iostream>

#include "Process.hpp"

namespace lp {
  static ssize_t handleToStream(HANDLE handle, std::stringstream &ss)
  {
    char buffer[4096];
    DWORD rd;
    ssize_t total = 0;

    do {
      if (!ReadFile(handle, buffer, sizeof(buffer), &rd, NULL))
        return -1;
      total += rd;
      std::cout << rd << std::endl;
      ss.write(buffer, rd);
    } while (rd == sizeof(buffer));
    return total;
  }

  void Process::pollStream(enum streamType stream) noexcept
  {
    DWORD status = WaitForSingleObject(_pipes[stream][0], _pollTimeout);
    if (status == WAIT_OBJECT_0 && handleToStream(_pipes[stream][0], _streams[stream - 1]) > 0 && _callbacks[stream - 1])
      _callbacks[stream - 1](*this, _streams[stream - 1]);
  }

  int Process::waitSingleStream(enum streamType stream) noexcept
  {
    while (isRunning()) {
      pollStream(stream);
    }
    return _status;
  }

  int Process::waitAll() noexcept
  {
    while (isRunning()) {
      for (uint8_t i = Stdout; i <= Stderr; ++i) {
        pollStream(static_cast<streamType>(i));
      }
    }
    return _status;
  }

  int Process::waitWithoutCallbacks() noexcept
  {
    DWORD status = WaitForSingleObject(_pi.hProcess, INFINITE);
    BOOL ret = GetExitCodeProcess(_pi.hProcess, &_status);
    CloseHandle(_pi.hProcess);
    CloseHandle(_pi.hThread);
    _isRunning = false;
    if (ret == false)
      return -1;
    return _status;
  }

  int Process::wait() noexcept
  {
    int ret;

    if (_callbacks[0] && _callbacks[1])
      ret = waitAll();
    else if (_callbacks[0])
      ret = waitSingleStream(Stdout);
    else if (_callbacks[1])
      ret = waitSingleStream(Stderr);
    else
      ret = waitWithoutCallbacks();
    for (uint8_t i = Stdout; i <= Stderr; ++i)
      if (isRedirecting(static_cast<enum streamType>(i)))
        pollStream(static_cast<enum streamType>(i));
    return ret;
  }
}
#endif
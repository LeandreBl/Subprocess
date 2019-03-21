#ifndef _WIN32
# include <unistd.h>
# include <sys/poll.h>
#endif

#include <iostream>
#include "Process.hpp"

namespace lp {
#ifndef _WIN32
  static ssize_t fdToStream(int fd, std::stringstream &stream)
  {
    char buffer[4096];
    ssize_t rd;
    ssize_t total = 0;

    do {
      rd = read(fd, buffer, sizeof(buffer));
      if (rd == -1)
        return -1;
      total += rd;
      stream.write(buffer, rd);
    } while (static_cast<size_t>(rd) == sizeof(buffer));
    return total;
  }

#endif
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
#ifdef _WIN32
    DWORD status = WaitForSingleObject(_pipes[stream][0], _pollTimeout);
    if (status == WAIT_OBJECT_0 && handleToStream(_pipes[stream][0], _streams[stream - 1]) > 0)
#else
    struct pollfd pfd;

    pfd.events = POLLIN;
    pfd.fd = _pipes[stream][0];
    if (poll(&pfd, 1, _pollTimeout) == 1 && fdToStream(_pipes[stream][0], _streams[stream - 1]) > 0)
#endif
      _callbacks[stream - 1](*this, _streams[stream - 1]);
  }

  int Process::waitSingleStream(enum streamType stream) noexcept
  {
    while (isRunning()) {
      std::cout << "wait..." << std::endl;
      pollStream(stream);
    }
    return _status;
  }

  int Process::waitAll() noexcept
  {
    while (isRunning())
      for (uint8_t i = Stdout; i <= Stderr; ++i)
        pollStream(static_cast<streamType>(i));
    return _status;
  }

  int Process::waitWithoutCallbacks() noexcept
  {
#ifdef _WIN32
    DWORD status = WaitForSingleObject(_pi.hProcess, INFINITE);
    BOOL ret = GetExitCodeProcess(_pi.hProcess, &_status);
    CloseHandle(_pi.hProcess);
    CloseHandle(_pi.hThread);
    _isRunning = false;
    if (ret == false)
      return -1;
    return _status;
#else
    int status;
    pid_t pid;

    pid = waitpid(_pid, &status, 0);
    _status = WEXITSTATUS(status);
    _isRunning = false;
    _pid = -1;
    if (pid == -1) {
      _status = EXIT_FAILURE;
      return -1;
    }
    return _status;
#endif
    return 0;
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
      if (isRedirecting(static_cast<enum streamType>(i)) && WaitForSingleObject(_pi.hProcess, 10) == 0)
        handleToStream(_pipes[i][0], _streams[i - 1]);
    return ret;
  }
}

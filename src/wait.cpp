#include <unistd.h>
#include <sys/poll.h>

#include "Process.hpp"

namespace lp {
  ssize_t fdToStream(int fd, std::stringstream &stream)
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

  void Process::pollStream(enum streamType stream) noexcept
  {
    struct pollfd pfd;

    pfd.events = POLLIN;
    pfd.fd = stream;
    if (poll(&pfd, 1, _pollTimeout) == 1 && fdToStream(_pipes[stream][0], _streams[stream - 1]) > 0)
      _callbacks[stream - 1](*this, _streams[stream - 1]);
  }

  int Process::waitSingleStream(enum streamType stream) noexcept
  {
    while (isRunning())
      pollStream(stream);
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
    int status;
    pid_t pid;

    pid = waitpid(_pid, &status, 0);
    _status = WEXITSTATUS(status);
    _isRunning = false;
    _pid = -1;
    if (pid == -1)
      return -1;
    return _status;
  }

  int Process::wait() noexcept
  {
    if (_callbacks[0] && _callbacks[1])
      return waitAll();
    else if (_callbacks[0])
      return waitSingleStream(Stdout);
    else if (_callbacks[1])
      return waitSingleStream(Stderr);
    return waitWithoutCallbacks();
  }
}

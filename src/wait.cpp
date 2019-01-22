#include <unistd.h>
#include <sys/select.h>

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
    } while (rd != sizeof(buffer));
    return total;
  }

  int Process::wait() noexcept
  {
    fd_set rfds;
    int nfds = 0;
    struct timeval tv = {
      .tv_sec = 0,
      .tv_usec = 1000,
    };

    FD_ZERO(&rfds);
    for (uint8_t i = stdout; i < stderr; ++i) {
      if (isRedirecting(i)) {
        FD_SET(i, &rfds);
        ++nfds;
      }
    }
    while (isRunning()) {
      int sel = select(nfds, &rfds, NULL, NULL, &tv);
      if (sel == -1)
        return -1;
      
    }
    return _status;
  }
}

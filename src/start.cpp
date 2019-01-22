#include <cstring>
#include <fcntl.h>
#include <unistd.h>

#include "Process.hpp"

namespace lp {
  static int getPath(const char *command, std::string &fillPath)
  {
    if (access(command, R_OK | X_OK) == 0) {
      fillPath = command;
      return (0);
    }
    const char *env = secure_getenv("PATH");
    if (env == nullptr)
      return (-1);
    std::string renv(env);
    char *token = std::strtok(const_cast<char *>(renv.c_str()), ":");
    while (token != nullptr) {
      fillPath = token;
      fillPath.push_back('/');
      fillPath += command;
      if (access(fillPath.c_str(), R_OK | X_OK) == 0)
        return (0);
      token = std::strtok(nullptr, ":");
    }
    return -1;
  }

  int Process::start() noexcept
  {
    std::string exePath;

    if (getPath(_parsedArgs[0], exePath) == -1)
      return (-1);
    for (uint8_t i = Stdin; i <= Stderr; ++i)
      if (isRedirecting(i) && pipe2(_pipes[i], O_CLOEXEC | O_NONBLOCK) == -1)
        return -1;
    if (isRedirecting(Stdin))
      std::swap(_pipes[Stdin][0], _pipes[Stdin][1]);
    _pid = fork();
    if (_pid != 0) {
      for (uint8_t i = Stdin; i <= Stderr; ++i)
        if (isRedirecting(i) && close(_pipes[i][1]) == -1)
          return -1;
      return _pid == -1 ? -1 : 0;
    }
    for (uint8_t i = Stdin; i <= Stderr; ++i) {
      if (isRedirecting(i) && (dup2(_pipes[i][1], i) == -1 || close(_pipes[i][0]) == -1))
        exit(EXIT_FAILURE);
    }
    chdir(_workingDirectory.c_str());
    return execve(exePath.c_str(), _parsedArgs.data(), environ);
  }
}
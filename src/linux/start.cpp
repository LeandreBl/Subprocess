#ifdef __linux__
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

#include "Process.hpp"

namespace lp {
  static int findFromPath(const char *command, const char *str, std::string &fillPath)
  {
    std::istringstream stream(str);
    while (stream) {
      std::getline(stream, fillPath, ':');
      fillPath.push_back('/');
      fillPath += command;
      if (access(fillPath.c_str(), R_OK | X_OK) == 0)
        return 0;
    }
    return -1;
  }

  static int getPath(const char *command, std::string &fillPath)
  {
    if (access(command, R_OK | X_OK) == 0) {
      fillPath = command;
      return 0;
    }
    const char *env = secure_getenv("PATH");
    if (env == nullptr)
      return -1;
    int ret = findFromPath(command, env, fillPath);
    return ret;
    return 0;
  }

  int Process::start() noexcept
  {
    if (_isRunning == true)
      return -1;
    std::string exePath;

    if (getPath(_parsedArgs[0], exePath) == -1)
      return -1;
    for (uint8_t i = Stdin; i <= Stderr; ++i)
      if (isRedirecting(static_cast<enum streamType>(i)) && pipe2(_pipes[i], O_CLOEXEC | O_NONBLOCK) == -1)
        return -1;
    if (isRedirecting(Stdin))
      std::swap(_pipes[Stdin][0], _pipes[Stdin][1]);
    _pid = fork();
    if (_pid != 0) {
      for (uint8_t i = Stdin; i <= Stderr; ++i)
        if (isRedirecting(static_cast<enum streamType>(i)) && close(_pipes[i][1]) == -1)
          return -1;
      return _pid == -1 ? -1 : 0;
    }
    for (uint8_t i = Stdin; i <= Stderr; ++i) {
      if (isRedirecting(static_cast<enum streamType>(i)) && (dup2(_pipes[i][1], i) == -1 || close(_pipes[i][0]) == -1))
        exit(EXIT_FAILURE);
    }
    if (chdir(_workingDirectory.c_str()))
      exit(EXIT_FAILURE);
    int ret = execve(exePath.c_str(), _parsedArgs.data(), environ);
    _isRunning = !ret;
    return ret;      
  }
}
#endif
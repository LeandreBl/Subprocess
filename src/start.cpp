#ifndef _WIN32
# include <cstring>
# include <unistd.h>
# include <fcntl.h>
#else
#endif
#include "Process.hpp"

#ifdef _WIN32
static const DWORD MAX_ENV_LEN = 32767;
#endif

namespace lp {
#ifndef _WIN32
  static int findFromPath(const char *command, const char *str, std::string &fillPath)
  {
    std::istringstream stream(env);
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
#endif

  int Process::start() noexcept
  {
    if (_isRunning == true)
      return -1;
#ifdef _WIN32
    ZeroMemory(&_si, sizeof(_si));
    _si.cb = sizeof(_si);
    ZeroMemory(&_pi, sizeof(_pi));
    _saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    _saAttr.bInheritHandle = TRUE; 
    _saAttr.lpSecurityDescriptor = NULL; 
    for (uint8_t i = Stdin; i <= Stderr; ++i)
      if (isRedirecting(static_cast<enum streamType>(i)) && !CreatePipe(&_pipes[i][0], &_pipes[i][1], &_saAttr, 0))
        return -1;
    if (isRedirecting(Stdin))
      std::swap(_pipes[Stdin][0], _pipes[Stdin][1]);
    for (uint8_t i = Stdin; i <= Stderr; ++i)
      if (isRedirecting(static_cast<enum streamType>(i)) && !SetHandleInformation(_pipes[i][0], HANDLE_FLAG_INHERIT, 0))
        return -1;
    if (isRedirecting(Stdin))
      _si.hStdInput = _pipes[Stdin][1];
    if (isRedirecting(Stdout))
      _si.hStdOutput = _pipes[Stdout][1];
    if (isRedirecting(Stderr))
      _si.hStdError = _pipes[Stderr][1];
    _si.dwFlags |= STARTF_USESTDHANDLES;
    if (!CreateProcess(NULL, const_cast<LPSTR>(_cmd.c_str()),  NULL, NULL, FALSE, 0, NULL, _workingDirectory.c_str(), &_si, &_pi))
      return -1;
    _isRunning = true;
    return 0;
#else
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
#endif
  }
}
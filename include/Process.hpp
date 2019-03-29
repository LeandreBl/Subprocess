#pragma once

#ifdef _WIN32
#  include <Windows.h>
#else
#  include <sys/wait.h>
#  include <sys/cdefs.h>
#endif

#include <cstdint>
#include <string>
#include <vector>
#include <functional>
#include <sstream>
#include <memory>

namespace lp {
class Process {
public:
  Process(const std::string &command = "", const std::string &workingDirectory = ".") noexcept;
  void setCommand(const std::string &command) noexcept;
  void setWorkingDir(const std::string &pathName) noexcept;
  void setStreamTimeout(int timeout) noexcept;
  void redirectStdin(bool does_redirect) noexcept;
  void redirectStdout(bool does_redirect) noexcept;
  void redirectStderr(bool does_redirect) noexcept;
  void onReadStdout(const std::function<void(Process &process, std::stringstream &stream)>
                            &callback) noexcept;
  void onReadStderr(const std::function<void(Process &process, std::stringstream &stream)>
                            &callback) noexcept;
  int64_t writeStdin(const std::string &string) const noexcept;
  template <typename T> int64_t writeStdin(const T &data) const noexcept
  {
    return writeStdin(&data, sizeof(data));
  }
  int64_t writeStdin(const void *data, size_t count) const noexcept;
  bool isRunning() noexcept;
  int start() noexcept; // voir si on throw pas un "command not found truc du genre" avec errno
  int wait() noexcept;
  std::stringstream &getStdout() noexcept;
  std::stringstream &getStderr() noexcept;
  int getStatus() const noexcept;
  const std::string &getCommand() const noexcept;

protected:
  enum streamType {
    Stdin = 0,
    Stdout = 1,
    Stderr = 2,
  };
  void pollStream(enum streamType stream) noexcept;
  int waitSingleStream(enum streamType stream) noexcept;
  int waitAll() noexcept;
  int waitWithoutCallbacks() noexcept;
  bool isRedirecting(enum streamType stream) const noexcept;
  std::stringstream &getStream(enum streamType stream) noexcept;
  void
  onRead(enum streamType stream,
         const std::function<void(Process &process, std::stringstream &stream)> &callback) noexcept;
  void redirectStream(enum streamType stream, bool does_redirect) noexcept;
  std::string _cmd;
  std::string _workingDirectory;
  std::stringstream _streams[2]; // sstream uses a lot of memory -!- it might be changed
  std::function<void(Process &process, std::stringstream &stream)> _callbacks[2];
#ifdef _WIN32
  void resetProcessInfos() noexcept;
  STARTUPINFO _si;
  PROCESS_INFORMATION _pi;
  SECURITY_ATTRIBUTES _saAttr;
  DWORD _status;
  HANDLE _pipes[3][2];
#else
  int _pipes[3][2];
  pid_t _pid;
  std::vector<char *> _parsedArgs;
  std::string _internalArgline;
  int _status;
#endif
  int _redirect;
  int _pollTimeout;
  bool _isRunning : 1;
};
} // namespace lp
#ifndef _LIBPROCESSPLUSPLUS_HPP_
# define _LIBPROCESSPLUSPLUS_HPP_

# include <sys/wait.h>
# include <sys/cdefs.h>

# include <string>
# include <vector>
# include <functional>
# include <sstream>
# include <memory>

//toremove
# include <iostream>

namespace lp {
  class Process {
    public:
      enum streamType {
        stdin = 0,
        stdout = 1,
        stderr = 2,
      };
      Process(const std::string &command = "", const std::string &workingDirectory = ".") noexcept;
      virtual ~Process() noexcept = default;
      void setCommand(const std::string &command) noexcept;
      void setWorkingDir(const std::string &pathName) noexcept;
      void redirect(enum streamType stream, bool does_redirect) noexcept;
      void onReceive(enum streamType stream, const std::function<void (Process &process, std::stringstream &stream)> &callback) noexcept;
      void toStdin(const std::string &string) noexcept;
      bool isRunning() noexcept;
      int start() noexcept; //voir si on throw pas un "command not found truc du genre" avec errno
      int wait() noexcept;
      std::stringstream &getStream(enum streamType) noexcept;
      int getStatus() const noexcept;
      bool isRedirecting(enum streamType stream) const noexcept;
      bool isRedirecting(int fd) const noexcept;
    protected:
      std::string _cmd;
      std::string _workingDirectory;
      std::stringstream _streams[2];
      std::function<void (Process &process, std::stringstream &stream)> _callbacks[2];
      int _pipes[3][2];
      std::string _internalArgline;
      std::vector<char *> _parsedArgs;
      pid_t _pid;
      int _status;
      bool _isRunning : 1;
      bool _redirect : 3;
  };
}

#endif // !_LIBPROCESSPLUSPLUS_HPP_
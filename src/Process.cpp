#include "Process.hpp"

namespace lp {
  Process::Process(const std::string &command, const std::string &workingDirectory) noexcept
  : _cmd(), _workingDirectory(), _internalArgline(), _parsedArgs(),
    _pid(-1), _status(EXIT_FAILURE), _redirect(false), _pollTimeout(10), _isRunning(false)
  {
    setWorkingDir(workingDirectory);
    setCommand(command);
  }
}

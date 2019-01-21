#include "Process.hpp"

namespace lp {
  Process::Process(const std::string &command, const std::string &workingDirectory) noexcept
  : _pid(-1), _status(EXIT_FAILURE), _isRunning(false), _redirect(false)
  {
    setWorkingDir(workingDirectory);
    setCommand(command);
  }
}
#include "Process.hpp"

#include <iostream>

namespace lp {
static void dummy(Process &process, std::stringstream &ss)
{
  (void)process;
  (void)ss;
}

Process::Process(const std::string &command, const std::string &workingDirectory) noexcept
        :
#ifdef __linux__
        _pid(-1)
        ,
#endif
        _status(EXIT_FAILURE)
        , _redirect(0)
        , _pollTimeout(10)
        , _isRunning(false)
{
  setWorkingDir(workingDirectory);
  setCommand(command);
}
} // namespace lp

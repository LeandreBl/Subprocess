#include "Process.hpp"

namespace lp {
void Process::setWorkingDir(const std::string &workingDirectory) noexcept
{
  _workingDirectory = workingDirectory;
}
} // namespace lp
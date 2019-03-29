#ifdef __linux__
#  include <cstring>

#  include "Process.hpp"

namespace lp {
void Process::setCommand(const std::string &command) noexcept
{
  char *sptr;

  _internalArgline = command;
  char *token = strtok_r(const_cast<char *>(_internalArgline.c_str()), " ", &sptr);
  while (token != nullptr) {
    _parsedArgs.push_back(token);
    token = strtok_r(nullptr, " ", &sptr);
  }
  _parsedArgs.push_back(nullptr);
  _cmd = command;
}
} // namespace lp
#endif
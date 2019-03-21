#ifdef __linux__
#include <cstring>

#include "Process.hpp"

namespace lp {
  void Process::setCommand(const std::string &command) noexcept
  {
    _internalArgline = command;
    char *token = std::strtok(const_cast<char *>(_internalArgline.c_str()), " ");
    while (token != nullptr) {
      _parsedArgs.push_back(token);
      token = std::strtok(nullptr, " ");
    }
    _parsedArgs.push_back(nullptr);
    _cmd = command;
  }
}
#endif
#include <cstring>

#include "Process.hpp"

namespace lp {
  void Process::setCommand(const std::string &command) noexcept
  {
    _cmd = command;
    _internalArgline = command;
    char *token = std::strtok(const_cast<char *>(_internalArgline.c_str()), " ");
    while (token != nullptr) {
      _parsedArgs.push_back(token);
      token = std::strtok(nullptr, " ");
    }
  }
}
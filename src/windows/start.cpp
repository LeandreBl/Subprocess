#ifdef _WIN32
#  include "Process.hpp"

static const DWORD MAX_ENV_LEN = 32767;
static const DWORD BUFSIZE = 4096;

namespace lp {
void Process::resetProcessInfos() noexcept
{
  SecureZeroMemory(&_si, sizeof(_si));
  SecureZeroMemory(&_pi, sizeof(_pi));
  _si.cb = sizeof(_si);
  _saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
  _saAttr.bInheritHandle = TRUE;
  _saAttr.lpSecurityDescriptor = NULL;
}

int Process::start() noexcept
{
  if (_isRunning == true)
    return -1;
  resetProcessInfos();
  for (uint8_t i = Stdin; i <= Stderr; ++i)
    if (isRedirecting(static_cast<enum streamType>(i))
        && !CreatePipe(&_pipes[i][0], &_pipes[i][1], &_saAttr, 0))
      return -1;
  if (isRedirecting(Stdin))
    std::swap(_pipes[Stdin][0], _pipes[Stdin][1]);
  for (uint8_t i = Stdin; i <= Stderr; ++i)
    if (isRedirecting(static_cast<enum streamType>(i))
        && !SetHandleInformation(_pipes[i][0], HANDLE_FLAG_INHERIT, 0))
      return -1;
  if (isRedirecting(Stdin))
    _si.hStdInput = _pipes[Stdin][1];
  if (isRedirecting(Stdout))
    _si.hStdOutput = _pipes[Stdout][1];
  if (isRedirecting(Stderr))
    _si.hStdError = _pipes[Stderr][1];
  _si.dwFlags |= STARTF_USESTDHANDLES;
  if (!CreateProcess(NULL, const_cast<LPSTR>(_cmd.c_str()), NULL, NULL, TRUE, 0,
                     GetEnvironmentStrings(), _workingDirectory.c_str(), &_si, &_pi))
    return -1;
  _isRunning = true;
  return 0;
}
} // namespace lp
#endif
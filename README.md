# Subprocess
A quick subprocess implementation in C++

`Creation of a simple subprocess, where we want to redirect the standard output`
```cpp
  lp::Process sp("ls -C -l --color=yes");

  sp.redirectStdout(true);
  sp.start();
  sp.wait();

  std::cout << sp.getStdout().str() << std::endl;
  std::cout << "Exit code: " << sp.getStatus() << std::endl;
```
A little more tricky ?

`Creation of a subprocess, where we redirect the standard output and error output, thus, we want to call a custom lambda function each time the subprocess wrote something in the error output`
```cpp
  lp::Process sp("ls ImAFileThatDoesNotExist");

  sp.redirectStdout(true);
  sp.redirectStderr(true);
  sp.onReadStderr([](lp::Process &process, std::stringstream &stream)
  {
    std::cout << "subprocess: \"" << stream.str() << "\"" << std::endl;
    //here we can clear the stream from it's content for example
    stream.str(std::string());
  });

  sp.start();
  sp.wait();

  std::cout << "command: \"" << sp.getCommand() << "\"" << std::endl;
  std::cout << "exit code: " << sp.getStatus() << std::endl;
```
Ah, you wanted to simulate an user input into the subprocess ? alright !

`Creation of a subprocess, where we redirect the standard input`
```cpp
  lp::Process sp("bash");

  sp.redirectStdin(true);
  sp.start();
  sp.writeStdin("ls && cd .. && mkdir ImAnEmptyFolder && cd ImAnEmptyFolder && touch file.txt\n");
  sp.writeStdin("exit\n");
  sp.wait();
```

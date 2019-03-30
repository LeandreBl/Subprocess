#define CATCH_CONFIG_MAIN
#include <single_include/catch2/catch.hpp>

#include "Process.hpp"

TEST_CASE("constructor", "[Process]")
{
  lp::Process sp("ls");

  REQUIRE(sp.getCommand() == "ls");
  REQUIRE(sp.getStatus() == EXIT_FAILURE);
  REQUIRE(sp.isRunning() == false);
}

TEST_CASE("redirect stdout", "[Process]")
{
  lp::Process sp("ls");

  sp.redirectStdout(true);
  REQUIRE(sp.start() == 0);
  REQUIRE(sp.wait() == 0);
  REQUIRE(sp.getStdout().str() == "CMakeCache.txt\nCMakeFiles\ncmake_install.cmake\nMakefile\ntests\n");
}

TEST_CASE("redirect stderr", "[Process]")
{
  
}
#define CATCH_CONFIG_MAIN
#include <single_include/catch2/catch.hpp>

#include "Process.hpp"

static void fillFromFile(std::string &str, FILE *src)
{
  char buffer[512];
  size_t rd;

  do {
    rd = fread(buffer, sizeof(*buffer), sizeof(buffer), src);
    str.reserve(str.size() + rd);
    str.append(buffer, rd);
  } while (rd == sizeof(buffer));
}

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
  FILE *f = popen(sp.getCommand().c_str(), "r");

  REQUIRE(f != nullptr);
  sp.redirectStdout(true);
  std::string output;
  fillFromFile(output, f);
  REQUIRE(sp.start() == 0);
  REQUIRE(sp.wait() == 0);
  REQUIRE(sp.getStdout().str() == output);
  fclose(f);
}

TEST_CASE("redirect stderr", "[Process]")
{
  lp::Process sp("ls aaaaaaaaaaaaaa");
  FILE *f = popen((std::string(sp.getCommand().c_str()) + " 2>&1").c_str(), "r");

  REQUIRE(f != nullptr);
  std::string output;
  fillFromFile(output, f);
  sp.redirectStderr(true);
  REQUIRE(sp.start() == 0);
  REQUIRE(sp.wait() == 2);
  REQUIRE(sp.getStderr().str() == output);
  fclose(f);
}
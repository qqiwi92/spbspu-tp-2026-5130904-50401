#include <iostream>
#include <string>
#include "commands.hpp"

int main()
{
  std::string current;
  using namespace levkin;
  Cmds cmds = getCmds();
  Database db;
  while (std::cin >> current) {
    try {
      cmds.at(current)(std::cin, std::cout, db);
    } catch (const std::logic_error& e) {
      std::cout << "<INVALID COMMAND: " << e.what() << ">\n";
    }
  }

  if (!std::cin.eof()) {
    std::cerr << "Bad input\n";
    return 1;
  }
}

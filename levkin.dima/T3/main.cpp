#include <iostream>
#include <string>
#include "commands.hpp"
#include "polygon.hpp"
#include <fstream>

int main(int argc, char** argv)
{
  if (argc < 2) {
    std::cerr << "missing input file\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "could not open file\n";
    return 1;
  }
  std::vector< levkin::Polygon > polygons;
  levkin::getPolygons(file, polygons);

  std::string current;
  using namespace levkin;
  Cmds cmds = getCmds();
  Database db;

  while (std::cin >> current) {
    try {
      if (cmds.find(current) == cmds.end()) {
        throw std::logic_error("Unknown command");
      }
      cmds.at(current)(std::cin, std::cout, polygons);
    } catch (const std::exception& e) {
      std::cout << "<INVALID COMMAND>\n";
      std::string dummy;
      std::getline(std::cin, dummy);
    }
  }

  if (!std::cin.eof() && std::cin.fail()) {
    std::cerr << "Bad input\n";
    return 1;
  }
  return 0;
}

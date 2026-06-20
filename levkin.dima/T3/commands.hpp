#ifndef COMMANDS
#define COMMANDS

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include "polygon.hpp"
namespace levkin {

using Database = const std::vector< Polygon >;
using cmd_t = std::function< void(std::istream&, std::ostream&, Database&) >;
using Cmds = std::unordered_map< std::string, cmd_t >;

void cmdArea(std::istream&, std::ostream&, Database&);
void cmdMax(std::istream&, std::ostream&, Database&);
void cmdMin(std::istream&, std::ostream&, Database&);
void cmdCount(std::istream&, std::ostream&, Database&);
void cmdMaxSeq(std::istream&, std::ostream&, Database&);
void cmdIntersections(std::istream&, std::ostream&, Database&);
std::string getWord(std::istream& in);

Cmds getCmds();
}

#endif

#ifndef COMMANDS
#define COMMANDS

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include "polygon.hpp"
namespace levkin {
class Note;

using Database = const std::vector< Polygon >;
using cmd_t = std::function< void(std::istream&, std::ostream&, Database&) >;
using Cmds = std::unordered_map< std::string, cmd_t >;

void note(std::istream& in, std::ostream& out, Database& db);

Cmds getCmds();
}

#endif

#include "commands.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace levkin {

void note(std::istream& in, std::ostream&, Database& db)
{
  //
}

Cmds getCmds()
{
  Cmds cmds;
  cmds["note"] = note;
  return cmds;
}
}

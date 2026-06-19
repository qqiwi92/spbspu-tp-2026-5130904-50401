#include "commands.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace levkin {
std::string getWord(std::istream& in)
{
  std::string s;
  if (!(in >> s)) {
    if (in.eof()) {
      return "";
    }
    in.clear();
    return "";
  }

  return s;
}

double areaOfPolygon(Polygon & p) {
    
}

void cmdArea(std::istream& in, std::ostream&, Database& db)
{
  std::string word = getWord(in);
  if (word == "EVEN") {
      
  }
}

Cmds getCmds()
{
  Cmds cmds;
  cmds["AREA"] = cmdArea;
  cmds["MAX"] = cmdMax;
  cmds["MIN"] = cmdMin;
  cmds["COUNT"] = cmdCount;
  cmds["INTERSECTIONS"] = cmdIntersections;
  cmds["COUNT"] = cmdCount;
  cmds["MAXSEQ"] = cmdMaxSeq;
  return cmds;
}
}

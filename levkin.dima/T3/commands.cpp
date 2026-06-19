#include "commands.hpp"
#include <iostream>
#include <iomanip>
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

void cmdArea(
    std::istream& in, std::ostream& out, const std::vector< Polygon >& db)
{
  std::string word = getWord(in);
  if (word == "MEAN" && db.empty()) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  std::vector< Polygon > filtered;
  filtered.reserve(db.size());

  using namespace detail;
  if (word == "EVEN") {
    std::copy_if(
        db.begin(), db.end(), std::back_inserter(filtered),
        std::bind(isEven, std::bind(getPolygonSize, std::placeholders::_1)));
  } else if (word == "ODD") {
    std::copy_if(
        db.begin(), db.end(), std::back_inserter(filtered),
        std::bind(isOdd, std::bind(getPolygonSize, std::placeholders::_1)));
  } else if (word == "MEAN") {
    std::copy(db.begin(), db.end(), std::back_inserter(filtered));
  } else {
    try {
      size_t targetSize = std::stoull(word);
      std::copy_if(
          db.begin(), db.end(), std::back_inserter(filtered),
          std::bind(
              isSizeEqual, std::bind(getPolygonSize, std::placeholders::_1),
              targetSize));
    } catch (...) {
      out << "<INVALID COMMAND>\n";
      return;
    }
  }

  double totalArea = polygonVectorAreaSum(filtered);

  detail::IOguard guard(out);
  out << std::fixed << std::setprecision(1);

  if (word == "MEAN") {
    out << (totalArea / db.size()) << "\n";
  } else {
    out << totalArea << "\n";
  }
}
void cmdMax(
    std::istream& in, std::ostream& out, const std::vector< Polygon >& db)
{
  std::string arg = getWord(in);
  if (db.empty() || (arg != "AREA" && arg != "VERTEXES")) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  detail::IOguard guard(out);
  using namespace detail;
  if (arg == "AREA") {
    auto it = std::max_element(db.begin(), db.end(), compareAreaLess);
    out << std::fixed << std::setprecision(1) << areaOfPolygon(*it) << "\n";
  } else if (arg == "VERTEXES") {
    auto it = std::max_element(db.begin(), db.end(), compareVertexesLess);
    out << detail::getPolygonSize(*it) << "\n";
  }
}

void cmdMin(
    std::istream& in, std::ostream& out, const std::vector< Polygon >& db)
{
  std::string arg = getWord(in);
  if (db.empty() || (arg != "AREA" && arg != "VERTEXES")) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  detail::IOguard guard(out);
  using namespace detail;
  if (arg == "AREA") {
    auto it = std::min_element(db.begin(), db.end(), compareAreaLess);
    out << std::fixed << std::setprecision(1) << areaOfPolygon(*it) << "\n";
  } else if (arg == "VERTEXES") {
    auto it = std::min_element(db.begin(), db.end(), compareVertexesLess);
    out << detail::getPolygonSize(*it) << "\n";
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

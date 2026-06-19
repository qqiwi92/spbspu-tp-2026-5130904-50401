#include "commands.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <algorithm>
#include <numeric>

namespace levkin {

using namespace std::placeholders;
const auto isEvenPolygon =
    std::bind(detail::isEven, std::bind(detail::getPolygonSize, _1));
const auto isOddPolygon =
    std::bind(detail::isOdd, std::bind(detail::getPolygonSize, _1));

auto makeSizeEqualPredicate(size_t target)
{
  return std::bind(
      detail::isSizeEqual, std::bind(detail::getPolygonSize, _1), target);
}

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

  if (word == "EVEN") {
    std::copy_if(
        db.begin(), db.end(), std::back_inserter(filtered), isEvenPolygon);
  } else if (word == "ODD") {
    std::copy_if(
        db.begin(), db.end(), std::back_inserter(filtered), isOddPolygon);
  } else if (word == "MEAN") {
    std::copy(db.begin(), db.end(), std::back_inserter(filtered));
  } else {
    try {
      size_t targetSize = std::stoull(word);
      std::copy_if(
          db.begin(), db.end(), std::back_inserter(filtered),
          makeSizeEqualPredicate(targetSize));
    } catch (...) {
      out << "<INVALID COMMAND>\n";
      return;
    }
  }

  double totalArea = detail::polygonVectorAreaSum(filtered);

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

void cmdCount(
    std::istream& in, std::ostream& out, const std::vector< Polygon >& db)
{
  std::string word = getWord(in);
  if (word.empty()) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  ptrdiff_t count = 0;

  if (word == "EVEN") {
    count = std::count_if(db.begin(), db.end(), isEvenPolygon);
  } else if (word == "ODD") {
    count = std::count_if(db.begin(), db.end(), isOddPolygon);
  } else {
    try {
      size_t targetSize = std::stoull(word);
      count = std::count_if(
          db.begin(), db.end(), makeSizeEqualPredicate(targetSize));
    } catch (...) {
      out << "<INVALID COMMAND>\n";
      return;
    }
  }

  detail::IOguard guard(out);
  out << count << "\n";
}
void cmdMaxSeq(
    std::istream& in, std::ostream& out, const std::vector< Polygon >& db)
{
  Polygon target;
  if (!(in >> target) || !detail::isEndOfLine(in)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  if (db.empty()) {
    detail::IOguard guard(out);
    out << 0 << "\n";
    return;
  }

  std::vector< bool > matches;
  matches.reserve(db.size());

  using namespace std::placeholders;
  std::transform(
      db.begin(), db.end(), std::back_inserter(matches),
      std::bind(
          static_cast< bool (*)(const Polygon&, const Polygon&) >(operator==),
          _1, target));

  size_t maxLen = detail::maxSequenceOfTrue(matches.begin(), matches.end());

  detail::IOguard guard(out);
  out << maxLen << "\n";
}

void cmdIntersections(std::istream& in, std::ostream& out, const std::vector< Polygon >& db)
{
  Polygon target;
  if (!(in >> target) || !detail::isEndOfLine(in)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  using namespace std::placeholders;
  ptrdiff_t count = std::count_if(db.begin(), db.end(),
                                  std::bind(checkPolygonIntersection, _1, std::cref(target)));

  detail::IOguard guard(out);
  out << count << "\n";
}
Cmds getCmds()
{
  Cmds cmds;
  cmds["AREA"] = cmdArea;
  cmds["MAX"] = cmdMax;
  cmds["MIN"] = cmdMin;
  cmds["COUNT"] = cmdCount;
  cmds["INTERSECTIONS"] = cmdIntersections;
  cmds["MAXSEQ"] = cmdMaxSeq;
  return cmds;
}
}

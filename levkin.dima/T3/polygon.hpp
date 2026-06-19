#ifndef POLYGON
#define POLYGON
#include <vector>
#include <iostream>

namespace levkin {
struct Point {
  int x, y;
};

struct Polygon {
  std::vector< Point > points;
};

void getPolygons(std::istream& input, std::vector< Polygon >& polygons);
std::istream& operator>>(std::istream& os, Point& p);
std::istream& operator>>(std::istream& os, Polygon& p);

namespace detail {
struct DelimiterIO {
  char val;
};

std::istream& operator>>(std::istream& input, DelimiterIO&& dest);
}
}

#endif

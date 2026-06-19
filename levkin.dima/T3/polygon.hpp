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

void getPolygons(std::istream & input, std::vector< Polygon > & polygons);
}


#endif
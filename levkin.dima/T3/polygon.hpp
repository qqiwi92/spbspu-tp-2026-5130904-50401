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
bool compareAreaLess(const Polygon& a, const Polygon& b);
bool compareVertexesLess(const Polygon& a, const Polygon& b);
double areaOfTriangle(const Polygon& p);
double areaOfPolygon(const Polygon& p);
double polygonVectorAreaSum(const std::vector< Polygon > v);
size_t getPolygonSize(const Polygon& poly);
bool isEven(size_t n);
bool isOdd(size_t n);
bool isSizeEqual(size_t n, size_t target);
struct DelimiterIO {
  char val;
};

std::istream& operator>>(std::istream& input, DelimiterIO&& dest);
bool isEndOfLine(std::istream& input);
void skipSpaces(std::istream& input);

class IOguard
{
public:
  explicit IOguard(std::basic_ios< char >& s);
  ~IOguard();

private:
  std::basic_ios< char >& s_;
  std::streamsize width_;
  char fill_;
  std::streamsize precision_;
  std::basic_ios< char >::fmtflags fmt_;
};
}
}

#endif

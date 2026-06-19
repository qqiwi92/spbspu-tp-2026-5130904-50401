#ifndef POLYGON
#define POLYGON
#include <vector>
#include <iostream>
#include <algorithm>
namespace levkin {
struct Point {
  int x, y;
};

struct Polygon {
  std::vector< Point > points;
};
struct Segment {
  Point p1, p2;
};
void getPolygons(std::istream& input, std::vector< Polygon >& polygons);
std::istream& operator>>(std::istream& os, Point& p);
std::istream& operator>>(std::istream& os, Polygon& p);
bool operator==(const Point& lhs, const Point& rhs);
bool operator==(const Polygon& lhs, const Polygon& rhs);

bool isCross(int min1, int max1, int min2, int max2);
bool isIntersect(const Segment& s1, const Segment& s2);
bool checkPolygonIntersection(const Polygon& lhs, const Polygon& rhs);
namespace detail {
Segment getSegment(const Polygon& poly, size_t i);
bool isSegmentIntersectPolygon(const Segment& seg, const Polygon& poly);
int areaSign(const Point& a, const Point& b, const Point& c);
bool compareAreaLess(const Polygon& a, const Polygon& b);
bool compareVertexesLess(const Polygon& a, const Polygon& b);
double areaOfTriangle(const Polygon& p);
double areaOfPolygon(const Polygon& p);
double polygonVectorAreaSum(const std::vector< Polygon > v);
size_t getPolygonSize(const Polygon& poly);
bool isEven(size_t n);
bool isOdd(size_t n);
bool isSizeEqual(size_t n, size_t target);
template < typename InputIt >
size_t maxSequenceOfTrue(InputIt first, InputIt last);
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

template < typename InputIt >
size_t levkin::detail::maxSequenceOfTrue(InputIt first, InputIt last)
{
  size_t maxLen = 0;
  auto it = first;

  while (it != last) {
    it = std::find(it, last, true);
    if (it == last) {
      break;
    }

    auto nextFalse = std::find(it, last, false);
    size_t currentLen = std::distance(it, nextFalse);

    if (currentLen > maxLen) {
      maxLen = currentLen;
    }

    it = nextFalse;
  }

  return maxLen;
}

#endif

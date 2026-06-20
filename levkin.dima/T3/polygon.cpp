#include "polygon.hpp"
#include <algorithm>
#include <iterator>
#include <numeric>
#include <functional>
#include <sstream>

namespace levkin {
void getPolygons(std::istream& input, std::vector< Polygon >& polygons)
{
  Polygon p;
  while (!input.eof()) {
    if (input >> p) {
      polygons.push_back(p);
    } else {
      if (input.eof()) {
        break;
      }
      input.clear();
      input.ignore(1);
    }
  }
}

bool operator==(const Point& lhs, const Point& rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator==(const Polygon& lhs, const Polygon& rhs)
{
  if (lhs.points.size() != rhs.points.size()) {
    return false;
  }
  return std::equal(lhs.points.begin(), lhs.points.end(), rhs.points.begin());
}

std::istream& operator>>(std::istream& input, Point& v)
{
  std::istream::sentry sentry(input);
  if (!sentry) {
    return input;
  }

  using del_t = detail::DelimiterIO;
  input >> del_t{'('} >> v.x >> del_t{';'} >> v.y >> del_t{')'};
  return input;
}

std::istream& operator>>(std::istream& input, Polygon& v)
{
  std::istream::sentry sentry(input);
  if (!sentry) {
    return input;
  }

  size_t amount = 0;
  if (!(input >> amount) || amount < 3) {
    input.setstate(std::ios::failbit);
    return input;
  }

  std::vector< Point > temp;
  temp.reserve(amount);

  for (size_t i = 0; i < amount; ++i) {
    Point p;
    if (input >> p) {
      temp.push_back(p);
    } else {
      input.setstate(std::ios::failbit);
      return input;
    }
  }

  v.points = std::move(temp);
  return input;
}

bool checkPolygonIntersection(const Polygon& lhs, const Polygon& rhs)
{
  if (lhs == rhs) {
    return true;
  }

  std::vector< size_t > indices(lhs.points.size());
  std::iota(indices.begin(), indices.end(), 0);

  using namespace std::placeholders;
  bool boundaryIntersect = std::any_of(
      indices.begin(), indices.end(),
      std::bind(
          detail::isSegmentIntersectPolygon,
          std::bind(detail::getSegment, std::cref(lhs), _1), std::cref(rhs)));

  if (boundaryIntersect) {
    return boundaryIntersect;
  }

  return detail::isPointInsidePolygon(lhs.points[0], rhs) ||
         detail::isPointInsidePolygon(rhs.points[0], lhs);
}
bool isCross(int min1, int max1, int min2, int max2)
{
  return std::max(min1, min2) <= std::min(max1, max2);
}

bool isIntersect(const Segment& s1, const Segment& s2)
{
  using namespace detail;
  return isCross(
             std::min(s1.p1.x, s1.p2.x), std::max(s1.p1.x, s1.p2.x),
             std::min(s2.p1.x, s2.p2.x), std::max(s2.p1.x, s2.p2.x)) &&
         isCross(
             std::min(s1.p1.y, s1.p2.y), std::max(s1.p1.y, s1.p2.y),
             std::min(s2.p1.y, s2.p2.y), std::max(s2.p1.y, s2.p2.y)) &&
         detail::areaSign(s1.p1, s1.p2, s2.p1) *
                 detail::areaSign(s1.p1, s1.p2, s2.p2) <=
             0 &&
         detail::areaSign(s2.p1, s2.p2, s1.p1) *
                 detail::areaSign(s2.p1, s2.p2, s1.p2) <=
             0;
}

namespace detail {
bool isPointInsidePolygon(const Point& p, const Polygon& poly)
{
  size_t n = poly.points.size();
  std::vector< size_t > indices(n);
  std::iota(indices.begin(), indices.end(), 0);

  auto count = std::count_if(indices.begin(), indices.end(), [&](size_t i) {
    const Point& v1 = poly.points[i];
    const Point& v2 = poly.points[(i + 1) % n];
    bool cond1 = (v1.y > p.y) != (v2.y > p.y);
    if (!cond1)
      return false;
    double intersectX =
        static_cast< double >(v2.x - v1.x) * (p.y - v1.y) / (v2.y - v1.y) +
        v1.x;
    return p.x < intersectX;
  });

  return count % 2 != 0;
}
std::istream& operator>>(std::istream& input, DelimiterIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry) {
    return input;
  }
  char c = 0;
  if (!(input >> c) || c != dest.val) {
    input.setstate(std::ios::failbit);
  }
  return input;
}
Segment getSegment(const Polygon& poly, size_t i)
{
  size_t n = poly.points.size();
  return Segment{poly.points[i], poly.points[(i + 1) % n]};
}
bool isSegmentIntersectPolygon(const Segment& seg, const Polygon& poly)
{
  std::vector< size_t > indices(poly.points.size());
  std::iota(indices.begin(), indices.end(), 0);

  using namespace std::placeholders;
  return std::any_of(
      indices.begin(), indices.end(),
      std::bind(isIntersect, seg, std::bind(getSegment, std::cref(poly), _1)));
}
int areaSign(const Point& a, const Point& b, const Point& c)
{
  long long area = static_cast< long long >(b.x - a.x) * (c.y - a.y) -
                   static_cast< long long >(b.y - a.y) * (c.x - a.x);
  if (area == 0)
    return 0;
  return (area > 0) ? 1 : -1;
}
bool compareAreaLess(const Polygon& a, const Polygon& b)
{
  return areaOfPolygon(a) < areaOfPolygon(b);
}
bool compareVertexesLess(const Polygon& a, const Polygon& b)
{
  return detail::getPolygonSize(a) < detail::getPolygonSize(b);
}
size_t getPolygonSize(const Polygon& poly) { return poly.points.size(); }
bool isEven(size_t n) { return n % 2 == 0; }
bool isOdd(size_t n) { return n % 2 != 0; }
bool isSizeEqual(size_t n, size_t target) { return n == target; }
void skipSpaces(std::istream& is)
{
  int ch = 0;
  while ((ch = is.peek()) != EOF) {
    if (ch == ' ' || ch == '\t' || ch == '\r') {
      is.get();
    } else {
      break;
    }
  }
}
bool isEndOfLine(std::istream& is)
{
  skipSpaces(is);
  int ch = is.peek();
  return ch == '\n' || ch == EOF;
}
IOguard::IOguard(std::basic_ios< char >& s)
    : s_(s), width_(s.width()), fill_(s.fill()), precision_(s.precision()),
      fmt_(s.flags())
{
}

IOguard::~IOguard()
{
  s_.width(width_);
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}

double areaOfTriangle(const Point& p0, const Point& p1, const Point& p2)
{
  return 0.5 *
         (p0.x * (p1.y - p2.y) + p1.x * (p2.y - p0.x) + p2.x * (p0.y - p1.y));
}

double areaOfPolygon(const Polygon& polygon)
{
  const auto& v = polygon.points;
  size_t n = v.size();
  if (n < 3) {
    return 0.0;
  }

  std::vector< size_t > indices(n);
  std::iota(indices.begin(), indices.end(), 0);

  std::vector< double > crossProducts;
  crossProducts.reserve(n);

  std::transform(
      indices.begin(), indices.end(), std::back_inserter(crossProducts),
      [&v, n](size_t i) {
        const Point& current = v[i];
        const Point& next = v[(i + 1) % n];
        return static_cast< double >(current.x * next.y) -
               static_cast< double >(next.x * current.y);
      });

  double totalArea =
      std::accumulate(crossProducts.begin(), crossProducts.end(), 0.0);

  return std::abs(totalArea) / 2.0;
}
double polygonVectorAreaSum(const std::vector< Polygon > v)
{
  std::vector< double > areas;
  areas.reserve(v.size());
  std::transform(v.begin(), v.end(), std::back_inserter(areas), areaOfPolygon);
  return std::accumulate(areas.begin(), areas.end(), 0.0);
}

}
}

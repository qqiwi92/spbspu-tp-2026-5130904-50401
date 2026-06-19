#include "polygon.hpp"

#include <algorithm>
#include <iterator>
#include <algorithm>
#include <numeric>

namespace levkin {
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

  std::copy_n(
      std::istream_iterator< Point >(input), amount, std::back_inserter(temp));

  if (!input || temp.size() < amount) {
    input.setstate(std::ios::failbit);
    return input;
  }

  v.points = std::move(temp);
  return input;
}

namespace detail {
bool isCross(int min1, int max1, int min2, int max2)
{
  return std::max(min1, min2) <= std::min(max1, max2);
}

int areaSign(const Point& a, const Point& b, const Point& c)
{
  long long area = static_cast< long long >(b.x - a.x) * (c.y - a.y) -
                   static_cast< long long >(b.y - a.y) * (c.x - a.x);
  if (area == 0)
    return 0;
  return (area > 0) ? 1 : -1;
}

bool isIntersect(const Segment& s1, const Segment& s2)
{
  return isCross(
             std::min(s1.p1.x, s1.p2.x), std::max(s1.p1.x, s1.p2.x),
             std::min(s2.p1.x, s2.p2.x), std::max(s2.p1.x, s2.p2.x)) &&
         isCross(
             std::min(s1.p1.y, s1.p2.y), std::max(s1.p1.y, s1.p2.y),
             std::min(s2.p1.y, s2.p2.y), std::max(s2.p1.y, s2.p2.y)) &&
         areaSign(s1.p1, s1.p2, s2.p1) * areaSign(s1.p1, s1.p2, s2.p2) <= 0 &&
         areaSign(s2.p1, s2.p2, s1.p1) * areaSign(s2.p1, s2.p2, s1.p2) <= 0;
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
}

double areaOfTriangle(Polygon& polygon)
{
  const std::vector< Point >& v = polygon.points;

  if (v.size() < 3)
    return 0.0;

  double area =
      (v[0].x * (v[1].y - v[2].y) + v[1].x * (v[2].y - v[0].y) +
       v[2].x * (v[0].y - v[1].y));

  return std::abs(area) / 2.0;
}

double areaOfTriangle(const Point& p0, const Point& p1, const Point& p2)
{
  return 0.5 *
         (p0.x * (p1.y - p2.y) + p1.x * (p2.y - p0.x) + p2.x * (p0.y - p1.y));
}

double areaOfPolygon(const Polygon& polygon)
{
  const auto& v = polygon.points;
  if (v.size() < 3) {
    return 0.0;
  }

  std::vector< size_t > indices(v.size() - 2);
  std::iota(indices.begin(), indices.end(), 2);

  std::vector< double > triangleAreas;
  triangleAreas.reserve(indices.size());

  std::transform(
      indices.begin(), indices.end(), std::back_inserter(triangleAreas),
      [&v](size_t i) { return areaOfTriangle(v[0], v[i - 1], v[i]); });
  double totalArea =
      std::accumulate(triangleAreas.begin(), triangleAreas.end(), 0.0);
  return std::abs(totalArea);
}
double polygonVectorAreaSum(std::vector< Polygon > v)
{
  std::vector< double > areas;
  std::transform(v.begin(), v.end(), std::back_inserter(areas), areaOfPolygon);
  double result = std::accumulate(areas.begin(), areas.end(), 0.0);
  return result;
}

}

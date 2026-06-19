#include "polygon.hpp"

#include <algorithm>
#include <iterator>

namespace levkin {

std::istream& operator>>(std::istream& input, Point& p)
{
  std::istream::sentry sentry(input);
  if (!sentry) {
    return input;
  }

  using del_t = detail::DelimiterIO;
  input >> del_t{'('} >> p.x >> del_t{';'} >> p.y >> del_t{')'};
  return input;
}

std::istream& operator>>(std::istream& input, Polygon& p)
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

  p.points = std::move(temp);
  return input;
}

namespace detail {
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
}

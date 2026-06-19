#include "polygon.hpp"

#include <algorithm>
#include <iterator>
#include <limits>

std::istream& levkin::operator>>(std::istream& input, Point& p)
{
  std::istream::sentry sentry(input);
  if (!sentry) {
    return input;
  }

  using del_t = detail::DelimiterIO;
  input >> del_t{'('} >> p.x >> del_t{';'} >> p.y >> del_t{')'};
  return input;
}

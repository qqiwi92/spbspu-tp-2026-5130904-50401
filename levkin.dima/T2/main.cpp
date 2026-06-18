#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

namespace levkin {
struct p_t {
  int x, y;
};
std::ostream& operator<<(std::ostream& os, p_t p);
std::istream& operator>>(std::istream& is, p_t& p);

bool operator<(p_t lhs, p_t rhs);
}

int main()
{
  using T = levkin::p_t;
  std::vector< T > data;

  using iit_t = std::istream_iterator< T >;
  using oit_t = std::ostream_iterator< T >;
  std::copy(iit_t{std::cin}, iit_t{}, std::back_inserter(data));

  std::sort(data.begin(), data.end());

  std::copy(data.begin(), data.end(), oit_t{std::cout, "\n"});
}

std::ostream& levkin::operator<<(std::ostream& os, levkin::p_t p)
{
  return os << '(' << p.x << ',' << ' ' << p.y << ')';
}

std::istream& levkin::operator>>(std::istream& is, p_t& p)
{
  if (!is) {
    return is;
  }
  char c = 0;
  is >> c;
  if (c != '(') {
    is.setstate(std::ios_base::failbit);
    return is;
  }
  int x = 0;
  is >> x;
  if (!is) {
    return is;
  }
  is >> c;
  if (c != ',') {
     is.setstate(std::ios_base::failbit);
     return is;
   }
  int y = 0;
  is >> y;
  if (!is) {
    return is;
  }

  is >> c;
  if (c != ')') {
    is.setstate(std::ios_base::failbit);
    return is;
  }
  if (is) {
    p = p_t{x, y};
  }
  return is;
}

bool levkin::operator<(p_t lhs, p_t rhs)
{
  if (lhs.x < rhs.x) {
    return true;
  } else if (lhs.x == rhs.x) {
    if (lhs.y < rhs.y) {
      return true;
    }
  }
  return false;
}

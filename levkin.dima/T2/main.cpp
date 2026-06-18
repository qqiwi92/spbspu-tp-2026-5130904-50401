#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

namespace levkin {
struct p_t {
  int x, y;
};

struct delimeter_t {
  char expected;
};

std::ostream& operator<<(std::ostream& os, p_t p);
std::istream& operator>>(std::istream& is, p_t& p);
std::istream& operator>>(std::istream& is, delimeter_t del);
bool operator<(p_t lhs, p_t rhs);
}

std::istream& check(std::istream& is, char ch)
{
  char c = 0;
  if (is >> c && c != ch) {
    is.setstate(std::ios_base::failbit);
  }
  return is;
}

std::istream& levkin::operator>>(std::istream& is, delimeter_t del)
{
  return check(is, del.expected);
}

std::ostream& levkin::operator<<(std::ostream& os, levkin::p_t p)
{
  std::ostream::sentry s(os);
  if (!s) {
    return os;
  }
  return os << '(' << p.x << "; " << p.y << ')';
}

std::istream& levkin::operator>>(std::istream& is, p_t& p)
{
  std::istream::sentry s(is);
  if (!s) {
    return is;
  }

  int x = 0, y = 0;
  using del_t = delimeter_t;

  is >> del_t{'('} >> x >> del_t{';'} >> y >> del_t{')'};

  if (is) {
    p = p_t{x, y};
  }
  return is;
}

bool levkin::operator<(p_t lhs, p_t rhs)
{
  if (lhs.x != rhs.x) {
    return lhs.x < rhs.x;
  }
  return lhs.y < rhs.y;
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

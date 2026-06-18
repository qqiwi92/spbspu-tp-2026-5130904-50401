#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

namespace levkin {
struct DataStruct {
  long long key1;
  std::pair< long long, unsigned long long > key2;
  std::string key3;
};

struct delimeter_t {
  char expected;
};

std::ostream& operator<<(std::ostream& os, DataStruct p);
std::istream& operator>>(std::istream& is, DataStruct& p);
std::istream& operator>>(std::istream& is, delimeter_t del);
bool operator<(DataStruct lhs, DataStruct rhs);
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

std::ostream& levkin::operator<<(std::ostream& os, levkin::DataStruct p)
{
  std::ostream::sentry s(os);
  if (!s) {
    return os;
  }
  return os << '(' << p.x << "; " << p.y << ')';
}

std::istream& levkin::operator>>(std::istream& is, DataStruct& p)
{
  std::istream::sentry s(is);
  if (!s) {
    return is;
  }

  int x = 0, y = 0;
  using del_t = delimeter_t;

  is >> del_t{'('} >> x >> del_t{';'} >> y >> del_t{')'};

  if (is) {
    p = DataStruct{x, y};
  }
  return is;
}

bool levkin::operator<(DataStruct lhs, DataStruct rhs)
{
  if (lhs.key1 != rhs.key1) {
    return lhs.key1 < rhs.key1;
  }
  if (lhs.key2 != rhs.key2) {
    return lhs.key2 < rhs.key2;
  }
  return lhs.key3.size() < rhs.key3.size();
}

int main()
{
  using levkin::DataStruct;
  std::vector< DataStruct > data;

  using iit_t = std::istream_iterator< DataStruct >;
  using oit_t = std::ostream_iterator< DataStruct >;

  std::copy(iit_t{std::cin}, iit_t{}, std::back_inserter(data));

  std::sort(data.begin(), data.end());

  std::copy(data.begin(), data.end(), oit_t{std::cout, "\n"});
}

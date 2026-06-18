#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

namespace levkin {
using Ratio = std::pair< long long, unsigned long long >;
struct SignedLongLong {
  long long val;
};
struct DataStruct {
  SignedLongLong key1;
  Ratio key2;
  std::string key3;
};

struct delimeter_t {
  char expected;
};

std::ostream& operator<<(std::ostream& os, DataStruct& p);
std::ostream& operator<<(std::ostream& os, SignedLongLong& p);
std::ostream& operator<<(std::ostream& os, Ratio& p);
std::istream& operator>>(std::istream& is, DataStruct& p);
std::istream& operator>>(std::istream& is, delimeter_t del);
bool operator<(DataStruct lhs, DataStruct rhs);
bool operator<(SignedLongLong lhs, SignedLongLong rhs);
bool operator==(SignedLongLong lhs, SignedLongLong rhs);
bool operator<(Ratio lhs, Ratio rhs);
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

std::ostream& levkin::operator<<(std::ostream& os, levkin::DataStruct& p)
{
  std::ostream::sentry s(os);
  if (!s) {
    return os;
  }
  return os << "(:" << p.key1 << ":" << p.key2 << ":" << '"' << p.key3 << '"'
            << ":)";
}

std::istream& levkin::operator>>(std::istream& is, DataStruct& p)
{
  std::istream::sentry s(is);
  if (!s) {
    return is;
  }

  using del_t = delimeter_t;
  SignedLongLong key1;
  Ratio key2;
  std::string key3;
  is >> del_t{'('} >> key1 >> del_t{';'} >> key2 >> del_t{';'} >> key3 >>
      del_t{')'};

  if (is) {
    p = DataStruct{x, y};
  }
  return is;
}

bool levkin::operator<(SignedLongLong lhs, SignedLongLong rhs)
{
  return lhs.val < rhs.val;
}

bool levkin::operator<(Ratio lhs, Ratio rhs)
{
  return (lhs.first * rhs.second) < (rhs.first * lhs.second);
}
bool levkin::operator==(SignedLongLong lhs, SignedLongLong rhs)
{
  return lhs.val == rhs.val;
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

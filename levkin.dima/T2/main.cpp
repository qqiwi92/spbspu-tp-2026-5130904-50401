#include <vector>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <cctype>

namespace levkin {
struct delimeter_t;
struct delimeter_span_t;
using Ratio = std::pair< long long, unsigned long long >;
using del_t = delimeter_t;
using del_span_t = delimeter_span_t;
struct SignedLongLong {
  long long val;
};
struct DataStruct {
  unsigned long long key1;
  Ratio key2;
  std::string key3;
};

struct DelimiterIO {
  char exp;
};

struct LabelIO {
  std::string exp;
};

struct UllLitIO {
  unsigned long long& ref;
};

struct RatLspIO {
  Ratio& ref;
};

struct StringIO {
  std::string& ref;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
    return in;
  char c = ' ';
  in >> c;
  if (in && c != dest.exp) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& check(std::istream& is, char ch)
{
  std::istream::sentry s(is);
  if (s) {
    if (is.peek() != ch) {
      is.setstate(std::ios_base::failbit);
    } else {
      is.get();
    }
  }
  return is;
}

std::istream& checkIgnoreCase(std::istream& is, char ch)
{
  std::istream::sentry s(is);
  if (s) {
    auto lowerC = std::tolower(static_cast< unsigned char >(is.peek()));
    auto lowerCh = std::tolower(static_cast< unsigned char >(ch));

    if (lowerC != lowerCh) {
      is.setstate(std::ios_base::failbit);
    } else {
      is.get();
    }
  }
  return is;
}

std::istream& checkSpan(std::istream& is, std::string& str)
{
  for (char c : str) {
    checkIgnoreCase(is, c);
  }
  return is;
}

std::istream& levkin::operator>>(std::istream& is, delimeter_t del)
{
  return check(is, del.expected);
}

std::istream& levkin::operator>>(std::istream& is, del_span_t del)
{
  return checkSpan(is, del.expected);
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
std::istream& levkin::operator>>(std::istream& is, SignedLongLong& p)
{
  std::istream::sentry s(is);
  if (!s) {
    return is;
  }
  SignedLongLong key;
  is >> key >> del_span_t{"ull"};
  return is;
}
std::istream& levkin::operator>>(std::istream& is, DataStruct& p)
{
  std::istream::sentry s(is);
  if (!s) {
    return is;
  }

  SignedLongLong key1;
  Ratio key2;
  std::string key3;
  is >> del_t{'('} >> key1 >> del_t{';'} >> key2 >> del_t{';'} >> key3 >>
      del_t{')'};

  if (is) {
    p = DataStruct{key1, key2, key3};
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

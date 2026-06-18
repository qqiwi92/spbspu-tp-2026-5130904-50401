#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <iomanip>

namespace levkin {
using Ratio = std::pair< long long, unsigned long long >;

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

std::istream& operator>>(std::istream& in, LabelIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
    return in;
  for (char exp_c : dest.exp) {
    char c = ' ';
    in >> c;
    if (in && std::tolower(c) != std::tolower(exp_c)) {
      in.setstate(std::ios::failbit);
      break;
    }
  }
  return in;
}

std::istream& operator>>(std::istream& in, UllLitIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
    return in;
  unsigned long long val;
  if (!(in >> val))
    return in;
  if (!(in >> LabelIO{"ull"}))
    return in;
  dest.ref = val;
  return in;
}

std::istream& operator>>(std::istream& in, RatLspIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
    return in;
  in >> DelimiterIO{'('} >> DelimiterIO{':'} >> LabelIO{"n"} >> dest.ref.first;
  in >> DelimiterIO{':'} >> LabelIO{"d"} >> dest.ref.second;
  in >> DelimiterIO{')'};
  return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
    return in;
  return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
}

std::istream& operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
    return in;

  DataStruct input;
  in >> DelimiterIO{'('} >> DelimiterIO{':'};

  for (int i = 0; i < 3; ++i) {
    std::string key;
    std::getline(in, key, ' ');
    if (key == "key1") {
      in >> UllLitIO{input.key1};
    } else if (key == "key2") {
      in >> RatLspIO{input.key2};
    } else if (key == "key3") {
      in >> StringIO{input.key3};
    } else {
      in.setstate(std::ios::failbit);
    }

    if (i < 2) {
      in >> DelimiterIO{':'};
    }
  }

  in >> DelimiterIO{':'} >> DelimiterIO{')'};
  if (in) {
    dest = input;
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
    return out;
  out << "(:key1 " << src.key1 << "ull";
  out << ":key2 (:N " << src.key2.first << ":D " << src.key2.second << ":)";
  out << ":key3 \"" << src.key3 << "\":)";
  return out;
}

bool compareDataStruct(const DataStruct& a, const DataStruct& b)
{
  if (a.key1 != b.key1)
    return a.key1 < b.key1;
  if (a.key2 != b.key2)
    return a.key2 < b.key2;
  return a.key3.length() < b.key3.length();
}
}

int main()
{
  using namespace levkin;
  std::vector< DataStruct > data;

  std::copy(
      std::istream_iterator< DataStruct >(std::cin),
      std::istream_iterator< DataStruct >(), std::back_inserter(data));

  if (std::cin.fail() && !std::cin.eof()) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }

  std::sort(data.begin(), data.end(), compareDataStruct);

  std::copy(
      data.begin(), data.end(),
      std::ostream_iterator< DataStruct >(std::cout, "\n"));

  return 0;
}

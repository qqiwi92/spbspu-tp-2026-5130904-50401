#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

namespace levkin {
    struct p_t {
        int x,y;
    };
    std::ostream & operator<<(std::ostream& os, p_t p);
    std::istream & operator>>(std::istream& is, p_t p);

    bool operator<(p_t lhs, p_t rhs);
}

int main()
{
  std::vector< int > data;

  using iit_t = std::istream_iterator< int >;
  using oit_t = std::ostream_iterator< int >;
  std::copy(iit_t{std::cin}, iit_t{}, std::back_inserter(data));

  std::sort(data.begin(), data.end());

  std::copy(data.begin(), data.end(), oit_t{std::cout, "\n"});
}

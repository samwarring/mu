#ifndef INCLUDED_MU_DETAIL_TO_STRING_HPP
#define INCLUDED_MU_DETAIL_TO_STRING_HPP
#include <algorithm>
#include <string>

namespace mu::detail {

constexpr std::string to_string(std::intmax_t value) {
  std::string ret;
  bool negative = (value < 0);
  if (negative) {
    value *= -1;
  };
  while (value > 0) {
    ret += ('0' + value % 10);
    value /= 10;
  };
  if (negative) {
    ret += '-';
  }
  std::reverse(ret.begin(), ret.end());
  return ret;
}

constexpr std::string to_string(std::intmax_t num, std::intmax_t den) {
  if (den == 1) {
    return to_string(num);
  } else {
    return "(" + to_string(num) + "/" + to_string(den) + ")";
  }
}

} // namespace mu::detail

#endif
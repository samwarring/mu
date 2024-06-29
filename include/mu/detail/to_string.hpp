#ifndef INCLUDED_MU_DETAIL_TO_STRING_HPP
#define INCLUDED_MU_DETAIL_TO_STRING_HPP
#include <algorithm>
#include <string>

namespace mu::detail {

/// Format an integer to a string in base-10.
///
/// This is needed because std::to_string is not constexpr.
///
/// \param value The integer to be formatted.
///
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

/// Format a ratio to a string.
///
/// If the denominator is 1, only format the numerator. Otherwise, format both
/// numerator and denominator, and enclose them in parentheses.
///
/// \param num The numerator.
/// \param den The denominator.
///
constexpr std::string to_string(std::intmax_t num, std::intmax_t den) {
  if (den == 1) {
    return to_string(num);
  } else {
    return "(" + to_string(num) + "/" + to_string(den) + ")";
  }
}

} // namespace mu::detail

#endif
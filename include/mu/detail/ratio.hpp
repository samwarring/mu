#ifndef INCLUDED_MU_DETAIL_RATIO_HPP
#define INCLUDED_MU_DETAIL_RATIO_HPP
#include <cstdint>
#include <utility>

namespace mu::detail {

/// A constexpr-friendly ratio.
///
/// Unlike std::ratio, whose numerator and denominator are template parameters,
/// this class stores the numerator and denominator in the instance. Also,
/// unlike std::ratio, this class makes no attempt to "simplify" its value
/// unless explicitly requested by calling ::simplify. This minimizes the work
/// performed in constexpr contexts.
///
struct ratio {

  /// The ratio numerator. Since these ratios are primarily multiplied together,
  /// their default value is 1 (mulitplicative identity).
  std::intmax_t num = 1;

  /// The ratio denominator.
  std::intmax_t den = 1;

  /// Default construct to 1.
  constexpr ratio() = default;

  /// Copy constructor.
  constexpr ratio(const ratio &rhs) : num(rhs.num), den(rhs.den) {}

  /// Construct as a whole number.
  constexpr ratio(std::intmax_t numerator) : num(numerator), den(1) {}

  /// Construct with numerator and denominator.
  constexpr ratio(std::intmax_t numerator, std::intmax_t denominator)
      : num(numerator), den(denominator) {}

  /// Returns true if the ratio is non-zero.
  constexpr bool is_zero() const { return num == 0; }

  /// Returns true if the ratio is 1.
  constexpr bool is_identity() const { return num == den; }

  /// Returns negative copy of this ratio.
  constexpr ratio operator-() const { return ratio(-num, den); }

  /// Checks that ratios are identical. This does NOT try to simplify the
  /// ratios, which means that `ratio(1,2) == ratio(2,4)` is false.
  constexpr bool operator==(const ratio &rhs) const {
    return num == rhs.num && den == rhs.den;
  }

  /// Multiplies two ratios. Does not simplify the result.
  constexpr ratio operator*(const ratio &rhs) const {
    return ratio(num * rhs.num, den * rhs.den);
  }

  /// Multiplies self by a ratio. Does not simplify the result.
  constexpr ratio &operator*=(const ratio &rhs) {
    num *= rhs.num;
    den *= rhs.den;
    return *this;
  }

  /// Adds another ratio to this one. Does not simplify the result.
  constexpr ratio &operator+=(const ratio &rhs) {
    if (den == rhs.den) {
      num += rhs.num;
    } else {
      num = (num * rhs.den) + (rhs.num * den);
      den = den * rhs.den;
    }
    return *this;
  }

  /// Compute greatest common divisor for 2 positive integers using Euclid's
  /// Algorithm.
  constexpr static std::intmax_t gcd(std::intmax_t a, std::intmax_t b) {
    // Ensure a is the larger of the 2 input values.
    if (a < b) {
      std::swap(a, b);
    }
    for (std::intmax_t r = a % b; r != 0; r = a % b) {
      a = b;
      b = r;
    }
    return b;
  }

  /// Simplify this ratio by dividing its numerator and denominator by their
  /// greatest common divisor.
  constexpr void simplify() {
    if (den != 1) {
      std::intmax_t d = gcd(num < 0 ? -num : num, den);
      num /= d;
      den /= d;
    }
  }
};

} // namespace mu::detail

#endif
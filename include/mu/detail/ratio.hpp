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
  constexpr ratio(const ratio &rhs) = default;

  /// Construct as a whole number.
  constexpr ratio(std::intmax_t numerator) : num(numerator), den(1) {}

  /// Construct with numerator and denominator.
  constexpr ratio(std::intmax_t numerator, std::intmax_t denominator)
      : num(numerator), den(denominator) {}

  /// Returns true if the ratio is non-zero.
  constexpr bool is_zero() const { return num == 0; }

  /// Returns true if the ratio is 1.
  constexpr bool is_identity() const { return num == den; }

  /// Returns true if the ratio is a whole number.
  constexpr bool is_whole() const { return num % den == 0; }

  /// Returns true if the ratio is a negative value.
  constexpr bool is_negative() const { return (num < 0) ^ (den < 0); }

  // Copy-assignment
  constexpr ratio &operator=(const ratio &) = default;

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
    normalize_sign();
    if (num == 0) {
      den = 1;
    } else if (den != 1) {
      std::intmax_t d = gcd(num < 0 ? -num : num, den);
      num /= d;
      den /= d;
    }
  }

  /// Normalize the +/- signs on the numerator and denominator.
  ///
  /// If this ratio is a positive number, then this method ensures the numerator
  /// and denominator are both positive numbers.
  ///
  /// If this ratio is a negative number, then this method ensures that only
  /// the nuemrator is negative, and the denominator is positive.
  ///
  constexpr void normalize_sign() {
    if (den < 0) {
      // Could be +num/-den. Or could be -num/-den. Either way, we need to
      // multiply both num and den by -1.
      num = -num;
      den = -den;
    }
  }
};

} // namespace mu::detail

#endif
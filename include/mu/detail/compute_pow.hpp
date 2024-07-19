#ifndef INCLUDED_MU_DETAIL_COMPUTE_POW_HPP
#define INCLUDED_MU_DETAIL_COMPUTE_POW_HPP
#include <cstdint>
#include <limits>
#include <mu/detail/ratio.hpp>
#include <optional>

namespace mu::detail {

/// Returns true if the value represents positive or negative infinity.
constexpr bool is_infinity(long double value) {
  static_assert(std::numeric_limits<long double>::has_infinity);
  const long double INFINITY = std::numeric_limits<long double>::infinity();
  return value == INFINITY || value == -INFINITY;
}

/// Safely multiply two integers while checking for signed integer overflow.
///
/// The input integers must both be nonzero, but either or both may be negative.
///
/// \return nullopt if the multiplication would result in an overflow. In this
/// case, the multiplication is not performed which prevents undefined behavior.
///
constexpr std::optional<std::intmax_t> safe_mult(std::intmax_t a,
                                                 std::intmax_t b) {
  const std::intmax_t MAX = std::numeric_limits<std::intmax_t>::max();
  const std::intmax_t MIN = std::numeric_limits<std::intmax_t>::min();

  // Enforce that a >= b. This implies:
  //   1. If a < 0, then b < 0.
  //   2. If b > 0, then a > 0.
  if (a < b) {
    std::swap(a, b);
  }
  if (b < 0) {
    if ((a > 0) && (b < MIN / a)) {
      return std::nullopt;
    } else if (b == MIN) {
      if (a == 1) {
        return MIN;
      }
      return std::nullopt;
    } else if (a < 0 && (-b > MAX / -a)) {
      return std::nullopt;
    }
  } else if ((MAX / a) < b) {
    return std::nullopt;
  }

  return a * b;
}

/// Compute integer base raised to an integer power.
///
/// \return nullopt if performing the multiplication would cause a signed
/// integer overflow.
///
constexpr std::optional<std::intmax_t>
compute_whole_pow_int(std::intmax_t base, std::intmax_t exponent) {
  std::intmax_t result = 1;
  for (std::intmax_t i = 0; i < exponent; ++i) {
    if (auto checked = safe_mult(result, base)) {
      result = *checked;
    } else {
      return std::nullopt;
    }
  }
  return result;
}

/// Compute floating point base raised to an integer power.
///
/// \param exponent Must be non-negative.
/// \return result may be positive or negative infinity.
///
constexpr long double compute_whole_pow_float(long double base,
                                              std::intmax_t exponent) {
  long double result = 1.0l;
  for (std::intmax_t i = 0; i < exponent; ++i) {
    result *= base;
  }
  return result;
}

/// Computes the nth root, ⁿ√x, where n is a positive integer.
///
/// The result is computed via Newton's Method.
///
/// \param base The number being rooted (aka the "radicand"). Assume this value
/// is greater than 0.
/// \param n The index of the radical (e.g. n=2 is a square root). Assume this
/// value is greater than 0.
///
constexpr long double compute_nth_root(long double base, std::intmax_t n) {

  // Refresher on Newton's Method:
  // https://en.wikipedia.org/wiki/Newton%27s_method
  //
  //  - We want a value x, such that x = ⁿ√base.
  //
  //  - Re-arrange the above, so xⁿ - base = 0.
  //
  //  - In other words, if we graph f(x) = xⁿ - base, this function crosses
  //    the x-axis at the desired value.
  //
  //  - We find this x-intercept by making a guess g₀. This guess is not the
  //    x-intercept, but we can improve the guess by taking the derivative of
  //    f(x) at g₀, and finding where that tangent line crosses the x-axis. This
  //    becomes our next guess, and we continue this process until we reach the
  //    desired precision.
  //
  //  The formulas are:
  //    f(x)  = xⁿ - base
  //    f'(x) = nxⁿ⁻¹
  //    gᵢ₊₁ = gᵢ - f(gᵢ)/f'(gᵢ) = gᵢ - (gᵢⁿ - base) / (ngᵢⁿ⁻¹)

  long double prev_guess = 0.0;
  long double guess = 0.0;
  long double next_guess = base < 0 ? -1.0 : 1.0;
  long double n_f = static_cast<long double>(n);

  while (guess != next_guess && prev_guess != next_guess) {
    prev_guess = guess;
    guess = next_guess;
    long double guess_pow_n_1 = compute_whole_pow_float(guess, n - 1);
    long double guess_pow_n = guess_pow_n_1 * guess;
    next_guess = guess - ((guess_pow_n - base) / (n_f * guess_pow_n_1));
  }

  return guess;
}

/// Computes xᴿ where R is a ratio.
///
/// The ratio is simplified before performing any exponentiation or nth roots.
/// This means an input of `(-n)²ᐟ⁴` is invalid, as the input can be simplified
/// as `(-n)¹ᐟ²` which is an even root of a negative number.
///
/// \param base The base of the exponent. May be positive or negative.
/// \param exponent The rational exponent value. The exponent is allowed to be
/// positive or negative, but the denominator must not be 0.
/// \return nullopt if the expression is undefined (either dividing by 0, or
/// taking an even root of a negative number).
///
constexpr std::optional<long double> compute_rational_pow(long double base,
                                                          ratio exponent) {

  // Handle exponent == 0.
  if (exponent.is_zero()) {
    return 1.0;
  }

  // Handle negative exponents.
  if (exponent.is_negative()) {
    auto inverse = compute_rational_pow(base, -exponent);
    if (!inverse || *inverse == 0) {
      return std::nullopt;
    }
    return 1.0 / *inverse;
  }

  // Exponent is positive, but make sure num and den are positive numbers.
  if (exponent.num < 0 && exponent.den < 0) {
    exponent.num = -exponent.num;
    exponent.den = -exponent.den;
  }

  // Convert exponent a/b into a "mixed number" consisting of whole and
  // fractional parts. E.g. 5/2 becomes 2 + 1/2.
  std::intmax_t exponent_whole = exponent.num / exponent.den;
  ratio exponent_fraction{exponent.num % exponent.den, exponent.den};
  exponent_fraction.simplify();

  if (base < 0 && exponent_fraction.den % 2 == 0) {
    // Attempting an even root of a negative number. Answer is an imaginary
    // number, which is not supported!
    return std::nullopt;
  }

  // Compute the whole exponent portion.
  long double result_whole = compute_whole_pow_float(base, exponent_whole);

  // Compute the fractional exponent portion.
  long double result_fractional = compute_nth_root(base, exponent_fraction.den);
  result_fractional =
      compute_whole_pow_float(result_fractional, exponent_fraction.num);

  // Final result!
  long double result = result_whole * result_fractional;
  return result;
}

} // namespace mu::detail

#endif
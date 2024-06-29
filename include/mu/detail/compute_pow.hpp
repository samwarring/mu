#ifndef INCLUDED_MU_DETAIL_COMPUTE_POW_HPP
#define INCLUDED_MU_DETAIL_COMPUTE_POW_HPP
#include <cstdint>
#include <mu/detail/ratio.hpp>

namespace mu::detail {

/// Computes xⁿ, where n is a positive whole number.
///
/// \param base In practice, this is a positive prime number.
/// \param exponent A positive whole number.
///
template <class T>
constexpr T compute_whole_pow(T base, std::intmax_t exponent) {
  T ret = 1;
  for (std::intmax_t i = 0; i < exponent; i++) {
    ret *= base;
  }
  return ret;
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
  long double next_guess = 1.0;
  long double n_f = static_cast<long double>(n);

  while (guess != next_guess && prev_guess != next_guess) {
    prev_guess = guess;
    guess = next_guess;
    long double guess_pow_n_1 = compute_whole_pow(guess, n - 1);
    long double guess_pow_n = guess_pow_n_1 * guess;
    next_guess = guess - ((guess_pow_n - base) / (n_f * guess_pow_n_1));
  }

  return static_cast<double>(guess);
}

/// Computes xʳ where r is a ratio.
///
/// \param base The base of the exponent. Must be non-negative.
/// \param exponent The rational exponent value. The exponent is allowed to be
/// positive or negative.
///
constexpr long double compute_rational_pow(long double base, ratio exponent) {

  // Handle exponent == 0.
  if (exponent.is_zero()) {
    return 1.0;
  }

  // Handle negative exponents.
  if (exponent.is_negative()) {
    long double inverse = compute_rational_pow(base, -exponent);
    return 1.0 / inverse;
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

  // Compute the whole exponent portion.
  long double result_whole = compute_whole_pow(base, exponent_whole);

  // Compute the fractional exponent portion.
  long double result_fractional = compute_nth_root(base, exponent_fraction.den);
  result_fractional =
      compute_whole_pow(result_fractional, exponent_fraction.num);

  // Final result!
  long double result = result_whole * result_fractional;
  return result;
}

} // namespace mu::detail

#endif
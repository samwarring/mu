#ifndef INCLUDED_MU_DETAIL_COMPUTE_POW_HPP
#define INCLUDED_MU_DETAIL_COMPUTE_POW_HPP
#include <cstdint>
#include <mu/detail/ratio.hpp>

namespace mu::detail {

/// Computes (base ^ power), where power is positive. Traditionally, this
/// function is called `pow`, but that name is already used to apply exponents
/// to unit types.
///
/// \param base In practice, this is a positive prime number.
/// \param exponent A positive, non-zero exponent.
///
template <class T> constexpr T compute_pow(T base, std::intmax_t exponent) {
  T ret = 1;
  for (std::intmax_t i = 0; i < exponent; i++) {
    ret *= base;
  }
  return ret;
}

/// Computes rational powers, effectively handling nth roots.
///
template <class T> constexpr long double compute_pow(T base, ratio exponent) {

  // Break exponent into whole + part.
  std::intmax_t exponent_whole = exponent.num / exponent.den;
  std::intmax_t exponent_part = exponent.num % exponent.den;

  // Compute portion of result obtained from the whole exponent.
  T result_whole = compute_pow(base, exponent_whole);

  // Compute portion of result obtained from the partial exponent. The partial
  // result is base^(a/b) where a/b < 1. Compute this by base^(1/b)^a
  exponent.num = exponent_part;
  exponent.simplify();
  long double result_partial = static_cast<long double>(base);

  if (exponent.den != 1) {
    // If exponent is not a whole number, first compute the root
    // [x = ⁿ√(base)]  →  [xⁿ = base]  →  [xⁿ - base = 0]
    // Set up a function: f(x) = x^n - base.
    // Its derivative is: f'(x) = n * x^(n-1).
    // To derive the next guess: x_next = x - (x^n - base)/(n * x^(n-1))

    long double prev_guess = 0.0;
    long double guess = 0.0;
    long double next_guess = 1.0;
    long double base_f = static_cast<long double>(base);
    long double n_f = static_cast<long double>(exponent.den);

    while (guess != next_guess && prev_guess != next_guess) {
      prev_guess = guess;
      guess = next_guess;
      long double guess_pow_n_1 = compute_pow(guess, exponent.den - 1);
      long double guess_pow_n = guess_pow_n_1 * guess;
      next_guess = guess - ((guess_pow_n - base_f) / (n_f * guess_pow_n_1));
    }
    result_partial = guess;
  }
  result_partial = compute_pow(result_partial, exponent.num);

  // Finally the result is the product of the whole and partial results.
  long double result = result_whole * result_partial;
  return result;
}

} // namespace mu::detail

#endif
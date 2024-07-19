#ifndef INCLUDED_MU_DETAIL_ANALYSIS_HPP
#define INCLUDED_MU_DETAIL_ANALYSIS_HPP
#include <algorithm>
#include <mu/detail/compute_pow.hpp>
#include <mu/detail/concrete_factor.hpp>
#include <mu/detail/primes.hpp>
#include <mu/pow.hpp>
#include <mu/units.hpp>

namespace mu::detail {

/// analysis objects perform dimensional analysis between two unit expressions
/// and determine their relationship to each other.
///
/// The analysis itself is performed in the constructor, so the results are
/// always available from a constructed analysis object.
///
/// This is a constexpr class by design, so the dimensional analysis can be
/// performed at compile-time.
///
/// \tparam FromUnits Analyze a conversion from these units
/// \tparam ToUnits Analyze a conversion to these units.
///
template <units FromUnits, units ToUnits> struct analysis {

  /// True if `FromUnits` can be converted to `ToUnits`.
  bool is_convertible = true;

  /// True if `FromUnits` can be converted to `ToUnits` without any scaling
  /// at all.
  bool is_equivalent = true;

  /// True if `FromUnits` can be converted to `ToUnits` by multiplying the
  /// source quanity by an integer.
  bool is_int_convertible = true;

  /// If the conversion is convertible by an integer, this holds the integer
  /// conversion value.
  std::intmax_t int_conversion = 1;

  /// If the conversion is NOT convertible by an integer, this holds the
  /// floating-point conversion value.
  long double float_conversion = 1.0;

  /// The constructor performs dimensional analysis of the conversion from
  /// `FromUnits` to `ToUnits`.
  constexpr analysis() {
    // Divide FromUnits by ToUnits, and obtain concrete factors for each factor
    // in the quotient.
    using quotient = mult<FromUnits, pow<ToUnits, -1>>;
    using quotient_factors = typename unit_traits<quotient>::factors;
    using generator = concrete_factor_generator<quotient_factors>;
    auto concrete_factors = generator::make_concrete_factors();

    // Scan all the concrete factors. Sum exponents of factors with the same
    // base.
    std::array<concrete_factor, generator::max_id> combined_factors;
    for (const auto &f : concrete_factors) {
      combine_factors(combined_factors[f.base_id], f);
    }

    // Track the prime-factorization of rational factors.
    std::vector<prime_factor> prime_factors;

    // Scan the combined factors.
    for (const auto &f : combined_factors) {
      if (f.base_id == UNKNOWN_BASE_ID || f.exponent.is_zero()) {
        continue;
      }
      if (f.is_dimensional) {
        // Found a combined *dimensional* factor with a non-zero exponent.
        // FromUnits and ToUnits have different dimensions, and are not
        // convertible.
        set_not_convertible();
        return;
      }
      if (!f.is_rational_value) {
        // Found a combined *irrational* factor with a non-zero exponent. This
        // guarantees that FromUnits cannot be converted to ToUnits without
        // multiplying by some floating point conversion value.
        if (!scale_by_float(f.irrational_value, f.exponent)) {
          set_not_convertible();
          return;
        }
      } else {
        // Found a *rational* factor. Break it into prime factors.
        prime_factorize(prime_factors, f.rational_value, f.exponent);
      }
    }

    // Combine prime factors, and calculate their contribution to the unit
    // conversion value.
    combine_prime_factors(prime_factors);
    for (auto &f : prime_factors) {
      if (f.base == -1) {
        // The conversion includes a negative factor.
        if (!scale_by_negative_1(f.exponent)) {
          set_not_convertible();
          return;
        }
      } else if (!f.exponent.is_negative() && f.exponent.is_whole()) {
        // This factor only requires int conversion.
        if (!scale_by_int(f.base, f.exponent.num / f.exponent.den)) {
          set_not_convertible();
          return;
        }
      } else {
        // This factor requires float conversion.
        if (!scale_by_float(f.base, f.exponent)) {
          set_not_convertible();
          return;
        }
      }
    }

    // Determine if units are equivalent, and if they are int-convertible.
    if (float_conversion != 1.0) {
      is_equivalent = false;
      is_int_convertible = false;
      float_conversion *= int_conversion;
      if (is_infinity(float_conversion)) {
        set_not_convertible();
      }
      return;
    }
    if (int_conversion != 1) {
      is_equivalent = false;
    }
  }

private:
  /// Multiply the exponent of \p src into \p dst. If \p dst has an unkown
  /// base_id, then copy \p src into \p dst.
  ///
  constexpr static void combine_factors(concrete_factor &dst,
                                        const concrete_factor &src) {
    if (dst.base_id == src.base_id) {
      dst.exponent += src.exponent;
    } else {
      dst.base_id = src.base_id;
      dst.exponent = src.exponent;
      dst.is_dimensional = src.is_dimensional;
      dst.is_rational_value = src.is_rational_value;
      dst.rational_value = src.rational_value;
      dst.irrational_value = src.irrational_value;
    }
  }

  /// Puts this object in the "not convertible" state.
  constexpr void set_not_convertible() {
    is_convertible = false;
    is_equivalent = false;
    is_int_convertible = false;
    return;
  }

  /// Scale the integer conversion by an integer raised to an integer power
  ///
  /// \return false if the exponentiation or scaling would result in integer
  /// overflow.
  ///
  constexpr bool scale_by_int(std::intmax_t value, std::intmax_t exponent) {
    if (auto checked_pow = compute_whole_pow_int(value, exponent)) {
      if (auto checked_mult = safe_mult(int_conversion, *checked_pow)) {
        int_conversion = *checked_mult;
        return true;
      }
    }
    return false;
  }

  /// Scale the float conversion by a float raised to a rational exponent.
  ///
  /// \return false if the scaling is undefined (divide by 0, or even root of
  /// negative number).
  ///
  constexpr bool scale_by_float(long double value, ratio exponent) {
    if (auto checked_pow = compute_rational_pow(value, exponent)) {
      float_conversion *= *checked_pow;
      if (!is_infinity(float_conversion)) {
        return true;
      }
    }
    return false;
  }

  /// Scale the integer conversion by -1 raised to a rational power.
  ///
  /// We only scale by -1 if the exponent is odd.
  ///
  /// \return false if the expression is undefined (even root of -1).
  ///
  constexpr bool scale_by_negative_1(ratio exponent) {
    exponent.simplify();
    if (exponent.den % 2 == 0) {
      // Even root of negative number is undefined.
      return false;
    }
    std::intmax_t abs_exponent_num =
        exponent.num < 0 ? -exponent.num : exponent.num;
    if (abs_exponent_num % 2 == 1) {
      // -1 raised to an odd exponent is negative.
      int_conversion = -int_conversion;
    }
    return true;
  }
};

/// A pre-constructed analysis object for the provided unit expressions.
///
/// By defining a global pre-constructed analysis object, the analysis between
/// two unit expressions need not be performed more than once. Compile-time
/// concepts that depend on dimensional analysis will obtain those results from
/// the corresponding global `analysis_object`.
///
/// \tparam FromUnits Analyze a conversion from these units
/// \tparam ToUnits Analyze a conversion to these units.
///
template <units FromUnits, units ToUnits>
constexpr analysis<FromUnits, ToUnits> analysis_object;

} // namespace mu::detail

#endif
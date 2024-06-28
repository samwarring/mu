#ifndef INCLUDED_MU_DETAIL_ANALYSIS_HPP
#define INCLUDED_MU_DETAIL_ANALYSIS_HPP
#include <algorithm>
#include <mu/detail/compute_pow.hpp>
#include <mu/detail/concrete_factor.hpp>
#include <mu/detail/primes.hpp>
#include <mu/pow.hpp>
#include <mu/units.hpp>

namespace mu::detail {

template <units FromUnits, units ToUnits> struct analysis {
  bool is_convertible = true;
  bool is_equivalent = true;
  bool is_int_convertible = true;
  std::intmax_t int_conversion = 1;
  long double float_conversion = 1.0;

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
        is_convertible = false;
        is_equivalent = false;
        is_int_convertible = false;
        return;
      }
      if (!f.is_rational_value) {
        // Found a combined *irrational* factor with a non-zero exponent. This
        // guarantees that FromUnits cannot be converted to ToUnits without
        // multiplying by some floating point conversion value.
        float_conversion *= compute_pow(f.irrational_value, f.exponent);
      } else {
        // Found a *rational* factor. Break it into prime factors.
        prime_factorize(prime_factors, f.rational_value, f.exponent);
      }
    }

    // Combine prime factors
    simplify_prime_factors(prime_factors);
    for (const auto &f : prime_factors) {
      if (f.exponent.den == 1) {
        // This factor only requires int conversion.
        int_conversion *= compute_pow(f.base, f.exponent.num);
      } else {
        // This factor requires float conversion.
        float_conversion *= compute_pow(f.base, f.exponent);
      }
    }

    // Determine if units are equivalent, and if they are int-convertible.
    if (float_conversion != 1.0) {
      is_equivalent = false;
      is_int_convertible = false;
      float_conversion *= int_conversion;
    } else if (int_conversion != 1) {
      is_equivalent = false;
    }
  }

private:
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
};

template <units FromUnits, units ToUnits>
constexpr analysis<FromUnits, ToUnits> analyzed;

} // namespace mu::detail

#endif
#ifndef INCLUDED_MU_DETAIL_PRIMES_HPP
#define INCLUDED_MU_DETAIL_PRIMES_HPP
#include <algorithm>
#include <array>
#include <cstdint>
#include <mu/detail/ratio.hpp>
#include <vector>

namespace mu::detail {

constexpr std::size_t PRIMES_TABLE_SIZE = 100;

constexpr std::array<std::intmax_t, PRIMES_TABLE_SIZE> PRIMES_TABLE{
    2,   3,   5,   7,   11,  13,  17,  19,  23,  29,  31,  37,  41,  43,  47,
    53,  59,  61,  67,  71,  73,  79,  83,  89,  97,  101, 103, 107, 109, 113,
    127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197,
    199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
    283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379,
    383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
    467, 479, 487, 491, 499, 503, 509, 521, 523, 541,
};

// This many primes are tested at a time before checking if we can stop testing
// primes.
constexpr std::size_t PRIMES_TABLE_BATCH_SIZE = 4;
static_assert(PRIMES_TABLE_SIZE % PRIMES_TABLE_BATCH_SIZE == 0);

struct prime_factor {
  std::intmax_t base{2};
  ratio exponent{0};

  constexpr prime_factor() = default;

  constexpr prime_factor(std::intmax_t base) : base(base), exponent(0) {}

  constexpr prime_factor(std::intmax_t base, std::intmax_t exponent)
      : base(base), exponent(exponent, 1) {}

  constexpr prime_factor(std::intmax_t base, ratio exponent)
      : base(base), exponent(exponent) {}

  constexpr bool operator==(const prime_factor &rhs) const = default;

  /// True if the base of this factor is less than the base of `rhs`. This is
  /// useful for sorting prime factors so common factors become adjacent.
  constexpr bool operator<(const prime_factor &rhs) const {
    return base < rhs.base;
  }
};

constexpr void try_prime_factor(std::vector<prime_factor> &out,
                                std::intmax_t &value, ratio exponent,
                                std::intmax_t candidate_prime) {
  if (value % candidate_prime != 0) {
    // Prime does not divide value.
    return;
  }

  // Divide value by candidate prime as far as possible.
  prime_factor current(candidate_prime);
  do {
    current.exponent.num++;
    value /= candidate_prime;
  } while (value % candidate_prime == 0);

  // Push the prime factor. Recall the value being factored is already raised to
  // an outer exponent.
  current.exponent *= exponent;
  out.push_back(current);
}

constexpr void prime_factorize_whole_number(std::vector<prime_factor> &out,
                                            std::intmax_t &value,
                                            ratio exponent) {

  // Test primes in batches. Before each batch, check if the current prime is
  // too high to consider checking further.
  for (std::size_t i = 0; i < PRIMES_TABLE_SIZE; i += PRIMES_TABLE_BATCH_SIZE) {
    if (PRIMES_TABLE[i] * PRIMES_TABLE[i] > value) {
      // This batch contains primes too high to bother checking.
      break;
    }

    // Test each prime in the batch.
    const std::size_t batch_end = i + PRIMES_TABLE_BATCH_SIZE;
    for (std::size_t j = 0; j < batch_end; ++j) {
      try_prime_factor(out, value, exponent, PRIMES_TABLE[j]);
    }
  }

  // If there is any remaining value, that means no primes in the table could
  // divide this value. Let's **ASSUME** the remaining portion is a prime
  // number.
  if (value > 1) {
    out.emplace_back(value, exponent);
  }
}

/// Factorizes a number of the form `base ^ exponent` where `base` and
/// `exponents` are rational numbers. The resulting prime_factors are pushed
/// into an output vector. The factors of base's numerator preceed the factors
/// of base's denominator. No effort is made to simplify the resulting factors
/// (e.g. by combining factors with the same base).
///
/// The prime-factorization algorithm does NOT calculate prime numbers, but
/// instead sources candidate primes from the `PRIMES_TABLE` earlier in this
/// file. If an input is the product of primes not found in that table, then
/// those primes will NOT appear in the factorization.
///
/// This function assumes as preconditions:
///   1. base is a non-negative value
///   2. base's denominator is nonzero
///   3. exponent's denominator is nonzero
///
/// An example, consider: prime_factorize(out, ratio{89, 36}, ratio{2, 3}).
///   - The prime factors of 49 = 2^1 * 7^2
///   - The prime factors of 36 = 2^2 * 3^2
///   - The prime factors of 1/36 = 2^-2 * 3^-2
///   - The exponent is distributed across all factors of the base.
///   - The resulting factors, in order, are: {2*2/3, 7^4/3, 2^-4/3, 3^-4/3}.
///
constexpr void prime_factorize(std::vector<prime_factor> &out, ratio base,
                               ratio exponent) {
  prime_factorize_whole_number(out, base.num, exponent);
  prime_factorize_whole_number(out, base.den, -exponent);
}

/// Given an output vector from one or more calls to `prime_factorize`, modify
/// it so factors of the same base have their exponents combined. The combined
/// exponents are not simplified.
///
constexpr void combine_prime_factors(std::vector<prime_factor> &prime_factors) {
  if (prime_factors.empty()) {
    return;
  }

  // Combine factors with the same base.
  std::sort(prime_factors.begin(), prime_factors.end());
  auto it = prime_factors.begin();
  auto cur = it;
  ++it;
  bool combine = false;
  while (it != prime_factors.end()) {
    if (it->base == cur->base) {
      cur->exponent += it->exponent;
      combine = true;
      ++it;
      continue;
    }
    if (combine) {
      if (!cur->exponent.is_zero()) {
        ++cur;
      }
      it = prime_factors.erase(cur, it);
      combine = false;
    }
    cur = it;
    ++it;
  }
  if (combine) {
    if (!cur->exponent.is_zero()) {
      ++cur;
    }
    prime_factors.erase(cur, it);
  } else if (cur->exponent.is_zero()) {
    prime_factors.erase(cur, it);
  }

  // Simplify the combined exponents.
  //   for (auto &f : prime_factors) {
  //     f.exponent.simplify();
  //   }
}

} // namespace mu::detail

#endif
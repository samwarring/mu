#include "mu_test.hpp"

using mu::mult;
using mu::detail::composite_constant_value;
using mu::detail::concrete_factor;
using mu::detail::concrete_factor_base_id;
using mu::detail::concrete_factor_generator;
using mu::detail::ratio;
using mu::detail::unit_traits;
using mu::detail::UNKNOWN_BASE_ID;

CONSTEXPR_TEST(MuConcreteFactor, Empty) {
  using t = mult<>;
  using g = concrete_factor_generator<typename unit_traits<t>::factors>;
  constexpr auto fs = g::make_concrete_factors();
  static_assert(fs.size() == 0);
}

CONSTEXPR_TEST(MuConcreteFactor, BaseUnit) {
  using t = apples;
  using g = concrete_factor_generator<typename unit_traits<t>::factors>;
  constexpr auto fs = g::make_concrete_factors();
  static_assert(fs.size() == 1);

  constexpr concrete_factor f = fs[0];
  static_assert(f.base_id != UNKNOWN_BASE_ID);
  static_assert(f.exponent == ratio{1});
  static_assert(f.is_dimensional);

  // If base does not appear in the intial factors, then its base id is unknown.
  constexpr concrete_factor_base_id other_base_id = g::base_id<oranges>;
  static_assert(other_base_id == UNKNOWN_BASE_ID);
}

CONSTEXPR_TEST(MuConcreteFactor, CompositeConstant) {
  using t = universal_fruit_constant;
  using g = concrete_factor_generator<typename unit_traits<t>::factors>;
  constexpr auto fs = g::make_concrete_factors();
  static_assert(fs.size() == 3);

  // Expected factors
  using t0 = composite_constant_value<universal_fruit_constant>;
  using t1 = apples;
  using t2 = oranges;

  // Get base ids for the expected factors.
  constexpr auto i0 = g::base_id<t0>;
  constexpr auto i1 = g::base_id<t1>;
  constexpr auto i2 = g::base_id<t2>;

  // All base ids should be unique w.r.t. each other.
  static_assert(i0 != i1);
  static_assert(i1 != i2);
  static_assert(i0 != i2);

  // No base ids should be unknown.
  static_assert(i0 != UNKNOWN_BASE_ID);
  static_assert(i1 != UNKNOWN_BASE_ID);
  static_assert(i2 != UNKNOWN_BASE_ID);

  // All ids should be less than the max id
  static_assert(i0 < g::max_id);
  static_assert(i1 < g::max_id);
  static_assert(i2 < g::max_id);

  // Check f0
  constexpr concrete_factor f0 = fs[0];
  static_assert(f0.base_id == i0);
  static_assert(f0.exponent == ratio{1});
  static_assert(!f0.is_dimensional);
  static_assert(!f0.is_rational_value);
  static_assert(f0.irrational_value == universal_fruit_constant::value);

  // check f1
  constexpr concrete_factor f1 = fs[1];
  static_assert(f1.base_id == i1);
  static_assert(f1.exponent == ratio{1});
  static_assert(f1.is_dimensional);

  // check f2
  constexpr concrete_factor f2 = fs[2];
  static_assert(f2.base_id == i2);
  static_assert(f2.exponent == ratio{1});
  static_assert(f2.is_dimensional);
}
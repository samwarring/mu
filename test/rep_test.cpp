#include "mu_test.hpp"
#include <mu/mu.hpp>
#include <string>

CONSTEXPR_TEST(MuRep, BaseReps) {
  static_assert(mu::rep<short>);
  static_assert(mu::rep<int>);
  static_assert(mu::rep<float>);
  static_assert(mu::rep<double>);
}

CONSTEXPR_TEST(MuRep, NotReps) {
  static_assert(!mu::rep<int *>);
  static_assert(!mu::rep<std::string>);
}

CONSTEXPR_TEST(MuRep, LosslesslyCastable) {
  static_assert(mu::rep_losslessly_castable_to<int, int>);
  static_assert(mu::rep_losslessly_castable_to<short, int>);
  static_assert(mu::rep_losslessly_castable_to<float, double>);
  static_assert(mu::rep_losslessly_castable_to<short, double>);
}

CONSTEXPR_TEST(MuRep, LossiyCastable) {
  static_assert(mu::rep_lossily_castable_to<int, int>);
  static_assert(mu::rep_lossily_castable_to<double, int>);
}

CONSTEXPR_TEST(MuRep, LosslesslyScalable) {
  static_assert(mu::rep_losslessly_scalable_to<int, int, int>);
  static_assert(mu::rep_losslessly_scalable_to<int, double, int>);
  static_assert(mu::rep_losslessly_scalable_to<int, double, long double>);
  static_assert(mu::rep_losslessly_scalable_to<short, float, long double>);
}

CONSTEXPR_TEST(MuRep, NotLosslesslyScalable) {
  static_assert(!mu::rep_losslessly_scalable_to<int, int, long double>);
  static_assert(!mu::rep_losslessly_scalable_to<short, short, short>);
  static_assert(!mu::rep_losslessly_scalable_to<short, short, signed char>);
  static_assert(!mu::rep_losslessly_scalable_to<int, unsigned int, int>);
}

CONSTEXPR_TEST(MuRep, LossilyScalable) {
  static_assert(mu::rep_lossily_scalable_to<float, int, long double>);
  static_assert(mu::rep_lossily_scalable_to<int, unsigned, long double>);
}

/// Define a stub class as an example of a length-n vector of any data type. The
/// implementation of the class is (mostly) left empty, as these tests are only
/// checking concepts, not performing any kind of evaluation.
template <mu::base_rep Val, std::size_t N> struct myvec {

  template <class Scale>
  constexpr myvec<decltype(std::declval<Val>() * std::declval<Scale>()), N>
  operator*(Scale scale_value);
};

/// Define rep traits for the custom vector type. Supported lossless and lossy
/// casts are declared, but intentionally left un-implemented.
template <mu::base_rep Val, std::size_t N>
struct mu::rep_traits<myvec<Val, N>> {
  using self_type = myvec<Val, N>;
  using base_rep_type = Val;

  template <mu::base_rep FromVal>
  requires mu::rep_losslessly_castable_to<FromVal, Val>
  constexpr static self_type lossless_cast(const myvec<FromVal, N> &);

  template <mu::base_rep FromVal>
  requires mu::rep_losslessly_castable_to<FromVal, Val>
  constexpr static self_type lossless_cast(myvec<FromVal, N> &&);

  template <mu::base_rep FromVal>
  requires mu::rep_lossily_castable_to<FromVal, Val>
  constexpr static self_type lossy_cast(const myvec<FromVal, N> &);
};
using myvec3i = myvec<int, 3>;
using myvec3f = myvec<float, 3>;
using myvec3d = myvec<double, 3>;
using myvec4i = myvec<int, 4>;
using myvec4f = myvec<float, 4>;
using myvec4d = myvec<double, 4>;

CONSTEXPR_TEST(MuRep, CustomRepLosslesslyCastable) {
  static_assert(mu::rep_losslessly_castable_to<myvec3i, myvec3d>);
  static_assert(mu::rep_losslessly_castable_to<myvec3f, myvec3d>);
}

CONSTEXPR_TEST(MuRep, CustomRepNotLosslesslyCastable) {
  static_assert(!mu::rep_losslessly_castable_to<myvec3d, myvec3i>);
  static_assert(!mu::rep_losslessly_castable_to<myvec3i, myvec4i>);
}

CONSTEXPR_TEST(MuRep, CustomRepLossilyCastable) {
  static_assert(mu::rep_lossily_castable_to<myvec4d, myvec4i>);
}

CONSTEXPR_TEST(MuRep, CustomRepNotLossilyCastable) {
  // Even if cast would be lossy, not allowed to cast vectors of different
  // lengths.
  static_assert(!mu::rep_lossily_castable_to<myvec3i, myvec4i>);
}

CONSTEXPR_TEST(MuRep, CustomRepLosslesslyScalable) {
  static_assert(mu::rep_losslessly_scalable_to<myvec3i, myvec3i, int>);
  static_assert(mu::rep_losslessly_scalable_to<myvec4i, myvec4d, long double>);
  static_assert(mu::rep_losslessly_scalable_to<myvec3f, myvec3d, long double>);
}

CONSTEXPR_TEST(MuRep, CustomRepNotLosslesslyScalable) {
  static_assert(!mu::rep_losslessly_scalable_to<myvec3i, myvec3i, long double>);
  static_assert(!mu::rep_losslessly_scalable_to<myvec3i, myvec3i, long double>);
  static_assert(!mu::rep_losslessly_scalable_to<myvec3d, myvec3i, long double>);
  static_assert(!mu::rep_losslessly_scalable_to<myvec4d, myvec3d, float>);
}

CONSTEXPR_TEST(MuRep, CustomRepLossilyScalable) {
  static_assert(mu::rep_lossily_scalable_to<myvec3f, myvec3i, long double>);
}

CONSTEXPR_TEST(MuRep, CustomRepNotLossilyScalable) {
  static_assert(!mu::rep_lossily_scalable_to<myvec4d, myvec3d, float>);
  static_assert(!mu::rep_lossily_scalable_to<myvec4d, myvec3i, int>);
}
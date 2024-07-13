#include "mu_test.hpp"

TEST(MuQuantity, ConstructIntApplesFromValue) {
  mu::quantity<int, apples> a{5};
  ASSERT_EQ(a.value(), 5);
}

TEST(MuQuantity, CopyConstructIntApplesFromIntApples) {
  mu::quantity<int, apples> a{5};
  mu::quantity<int, apples> b{a};
  ASSERT_EQ(b.value(), 5);
}

TEST(MuQuantity, MoveConstructIntApplesFromIntApples) {
  mu::quantity<int, apples> a{5};
  mu::quantity<int, apples> b{std::move(a)};
  ASSERT_EQ(b.value(), 5);
}

TEST(MuQuantity, CopyAssignIntApplesFromIntApples) {
  mu::quantity<int, apples> a{5};
  mu::quantity<int, apples> b{1};
  b = a;
  ASSERT_EQ(b.value(), 5);
}

TEST(MuQuantity, MoveAssignIntApplesFromIntApples) {
  mu::quantity<int, apples> a{5};
  mu::quantity<int, apples> b{1};
  b = std::move(a);
  ASSERT_EQ(b.value(), 5);
}

TEST(MuQuantity, ConstructDoubleApplesFromIntApples) {
  mu::quantity<int, apples> a{5};
  mu::quantity<double, apples> b{a};
  ASSERT_EQ(b.value(), 5.0);
}

CONSTEXPR_TEST(MuQuantity, ConstructShortApplesFromIntApples) {
  static_assert(
      !mu::quantity_losslessly_convertible_to<int, apples, short, apples>);
}

TEST(MuQuantity, ConstructIntMilliApplesFromIntApples) {
  mu::quantity<int, apples> a{5};
  mu::quantity<int, mu::mult<std::milli, apples>> b{a};
  ASSERT_EQ(b.value(), 5000);
}

CONSTEXPR_TEST(MuQuantity, ConstructIntKiloApplesFromIntApples) {
  static_assert(
      !mu::quantity_losslessly_convertible_to<int, apples, int,
                                              mu::mult<std::kilo, apples>>);
}

TEST(MuQuantity, ConstructDoubleKiloApplesFromIntApples) {
  mu::quantity<int, apples> a{5};
  mu::quantity<double, mu::mult<std::kilo, apples>> b{a};
  ASSERT_EQ(b.value(), 0.005);
}

TEST(MuQuantity, CastIntApplesToIntKiloApples) {
  mu::quantity<int, apples> a{5400};
  auto b = mu::quantity_cast<int, mu::mult<std::kilo, apples>>(a);
  ASSERT_EQ(b.value(), 5);
}

TEST(MuQuantity, MoveCastIntApplesToIntKiloApples) {
  mu::quantity<int, apples> a{5400};
  auto b = mu::quantity_cast<int, mu::mult<std::kilo, apples>>(std::move(a));
  ASSERT_EQ(b.value(), 5);
}

TEST(MuQuantity, AddIntApplesToIntApples) {
  mu::quantity<int, apples> a{12};
  mu::quantity<int, apples> b{55};
  auto c = a + b;
  ASSERT_EQ(c.value(), 67);
}

TEST(MuQuantity, AddIntApplesToDoubleApples) {
  mu::quantity<int, apples> a{12};
  mu::quantity<double, apples> b{55.5};
  auto c = a + b;
  ASSERT_EQ(c.value(), 67.5);
}

TEST(MuQuantity, AddDoubleApplesToIntApples) {
  mu::quantity<double, apples> a{12.5};
  mu::quantity<int, apples> b{55};
  auto c = a + b;
  ASSERT_EQ(c.value(), 67.5);
}

TEST(MuQuantity, SubtractIntApplesFromIntApples) {
  mu::quantity<int, apples> a{10};
  mu::quantity<int, apples> b{3};
  auto c = a - b;
  ASSERT_EQ(c.value(), 7);
}

TEST(MuQuantity, MultiplyIntApplesByDoubleOranges) {
  mu::quantity<int, apples> a{10};
  mu::quantity<double, oranges> b{12.5};
  auto c = a * b;
  static_assert(mu::units_equivalent_to<typename decltype(c)::units_type,
                                        mu::mult<apples, oranges>>);
  ASSERT_EQ(c.value(), 125.0);
}

TEST(MuQuantity, MultiplyIntApplesByRawDouble) {
  mu::quantity<int, apples> a{10};
  mu::quantity b = a * 1.5;
  ASSERT_EQ(b.value(), 15.0);

  mu::quantity c = 0.5 * a;
  ASSERT_EQ(c.value(), 5.0);
}

TEST(MuQuantity, DivideIntApplesByDoubleOranges) {
  mu::quantity<int, apples> a{125};
  mu::quantity<double, oranges> b{10.0};
  auto c = a / b;
  static_assert(
      mu::units_equivalent_to<typename decltype(c)::units_type,
                              mu::mult<apples, mu::pow<oranges, -1>>>);
  ASSERT_EQ(c.value(), 12.5);
}

TEST(MuQuantity, DivideIntApplesByRawInt) {
  mu::quantity<int, apples> a{12};
  mu::quantity b_actual = a / 2;
  mu::quantity<int, apples> b_expected{6};
  ASSERT_EQ(b_actual, b_expected);
}

TEST(MuQuantity, DivideRawIntByIntApples) {
  mu::quantity<int, apples> a{2};
  mu::quantity b_actual = 10 / a;
  mu::quantity<int, mu::pow<apples, -1>> b_expected{5};
  ASSERT_EQ(b_actual, b_expected);
}

TEST(MuQuantity, CompareEqualApplesToApples) {
  mu::quantity<int, apples> i{12};
  mu::quantity<double, apples> d{12};
  ASSERT_EQ(i, d);
}

TEST(MuQuantity, CompareNotEqualApplesToApples) {
  mu::quantity<int, apples> a{12};
  mu::quantity<int, apples> b{13};
  ASSERT_NE(a, b);
}

TEST(MuQuantity, CompareLessApplesToApples) {
  mu::quantity<int, apples> a{12};
  mu::quantity<int, apples> b{13};
  ASSERT_LT(a, b);
}

TEST(MuQuantity, CompareLessEqualApplesToApples) {
  mu::quantity<int, apples> a{12};
  mu::quantity<int, apples> b{13};
  ASSERT_LE(a, b);
  ASSERT_LE(b, b);
}

TEST(MuQuantity, CompareGreaterApplesToApples) {
  mu::quantity<int, apples> a{13};
  mu::quantity<int, apples> b{12};
  ASSERT_GT(a, b);
}

TEST(MuQuantity, CompareGreaterEqualApplesToApples) {
  mu::quantity<int, apples> a{13};
  mu::quantity<int, apples> b{12};
  ASSERT_GE(a, b);
  ASSERT_GE(a, a);
}

TEST(MuQuantity, ConstructFromReferences) {
  auto a = 12 * apples{};
  auto b = 3 * apples{} * 4;
  ASSERT_EQ(a, b);

  auto c = a * oranges{};
  auto d = oranges{} * b;
  auto e = 12 * (apples{} * oranges{});
  ASSERT_EQ(c, d);
  ASSERT_EQ(c, e);
  ASSERT_EQ(d, e);

  auto f = c / oranges{};
  ASSERT_EQ(f, a);
}
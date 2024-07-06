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

TEST(MuQuantity, MultiplyIntApplesByDoubleOranges) {
  mu::quantity<int, apples> a{10};
  mu::quantity<double, oranges> b{12.5};
  auto c = a * b;
  static_assert(mu::units_equivalent_to<typename decltype(c)::units_type,
                                        mu::mult<apples, oranges>>);
  ASSERT_EQ(c.value(), 125.0);
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
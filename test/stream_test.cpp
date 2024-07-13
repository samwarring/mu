#include "mu_test.hpp"
#include <sstream>

struct MuStreamTest : public testing::Test {
  mu::format_options opts;
  std::ostringstream stream;
};

TEST_F(MuStreamTest, Default) {
  auto q = 3 * apples{} * mu::pow<oranges, -2>{};
  stream << q;
  ASSERT_EQ(stream.str(), "3 apples * oranges^-2");
}

TEST_F(MuStreamTest, Format) {
  auto q = 3 * apples{} * oranges{};
  opts.labels = mu::format_options::label_type::symbols;
  opts.mult_sep = " ⨉ ";
  stream << mu::stream::format(opts) << q;
  ASSERT_EQ(stream.str(), "3 🍎 ⨉ 🍊");
}

TEST_F(MuStreamTest, Symbols) {
  auto q = 3 * apples{};
  stream << mu::stream::symbols << q;
  ASSERT_EQ(stream.str(), "3 🍎");
}

TEST_F(MuStreamTest, MultSep) {
  auto q = 3 * apples{} * basket{} * oranges{};
  stream << mu::stream::mult_sep(" • ") << q;
  ASSERT_EQ(stream.str(), "3 apples • basket_of_oranges");
}

TEST_F(MuStreamTest, Labels) {
  auto q = 4 * oranges{};
  stream << mu::stream::labels(mu::format_options::label_type::symbols) << q;
  ASSERT_EQ(stream.str(), "4 🍊");
}

TEST_F(MuStreamTest, SymbolsThenNames) {
  auto q = 2 * apples{} * oranges{};
  stream << mu::stream::symbols << q << " | " << mu::stream::names << q;
  ASSERT_EQ(stream.str(), "2 🍎 * 🍊 | 2 apples * oranges");
}

TEST_F(MuStreamTest, Reset) {
  auto q = 5 * apples{} * oranges{};
  stream << mu::stream::symbols << mu::stream::mult_sep(" x ") << q << " | "
         << mu::stream::reset << q;
  ASSERT_EQ(stream.str(), "5 🍎 x 🍊 | 5 apples * oranges");
}
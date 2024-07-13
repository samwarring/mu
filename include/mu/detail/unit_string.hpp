#ifndef INCLUDED_MU_DETAIL_UNIT_STRING_HPP
#define INCLUDED_MU_DETAIL_UNIT_STRING_HPP
#include <cstdint>
#include <mu/detail/ratio.hpp>
#include <mu/detail/to_string.hpp>
#include <string>
#include <string_view>

namespace mu {

/// Options specify how units should be displayed when formatted as a string.
struct format_options {

  /// Allowed options for displaying named units.
  enum class label_type {
    /// Display named units using their full names.
    names,
    /// Display named units using their symbols.
    symbols
  };

  /// Determines how to display named units.
  label_type labels = label_type::names;

  /// Separator used to diplay units that are multiplied together.
  const char *mult_sep = " * ";
};

struct labels {
  const char *name = "";
  const char *symbol = "";
  bool is_prefix = false;
};

namespace detail {

class unit_string {
public:
  constexpr unit_string(format_options opts)
      : opts_{std::move(opts)}, subexpression_stack_{1} {}

  constexpr void push() { subexpression_stack_.emplace_back(); }

  constexpr void pop() {
    subexpression top = std::move(back());
    subexpression_stack_.pop_back();
    if (!back().str.empty() && !back().ends_with_prefix) {
      back().str += opts_.mult_sep;
      back().pow_requires_parens = true;
    }
    back().str += top.str;
    back().ends_with_prefix = top.ends_with_prefix;
    back().pow_requires_parens =
        back().pow_requires_parens || top.pow_requires_parens;
  }

  constexpr void multiply(const labels &named_unit) {
    if (!back().str.empty() && !back().ends_with_prefix) {
      back().str += opts_.mult_sep;
      back().pow_requires_parens = true;
    }
    switch (opts_.labels) {
    case format_options::label_type::names:
      back().str += named_unit.name;
      break;
    case format_options::label_type::symbols:
      back().str += named_unit.symbol;
      break;
    default:
      back().str += named_unit.name;
    }
    back().ends_with_prefix = named_unit.is_prefix;
  }

  constexpr void multiply(ratio constant_value) {
    if (!back().str.empty()) {
      back().str += opts_.mult_sep;
      back().pow_requires_parens = true;
    }
    back().str += to_string(constant_value);
    back().ends_with_prefix = false;
  }

  constexpr void pow(ratio exponent) {
    std::string suffix = "^" + to_string(exponent);
    if (back().pow_requires_parens) {
      back().str = "(" + back().str + ")" + suffix;
    } else {
      back().str += suffix;
    }
    back().pow_requires_parens = true;
  }

  constexpr std::string str() const {
    std::string ret;
    for (const auto &subexpr : subexpression_stack_) {
      ret += subexpr.str;
    }
    return ret;
  }

private:
  format_options opts_;

  struct subexpression {
    std::string str;
    bool ends_with_prefix = false;
    bool pow_requires_parens = false;
  };
  std::vector<subexpression> subexpression_stack_;

private:
  constexpr subexpression &back() { return subexpression_stack_.back(); }

  constexpr const subexpression &back() const {
    return subexpression_stack_.back();
  }

  constexpr static std::string to_string(std::intmax_t value) {
    std::string ret;
    bool negative = (value < 0);
    if (negative) {
      value *= -1;
    };
    while (value > 0) {
      ret += ('0' + value % 10);
      value /= 10;
    };
    if (negative) {
      ret += '-';
    }
    std::reverse(ret.begin(), ret.end());
    return ret;
  }

  constexpr static std::string to_string(ratio value) {
    if (value.den == 1) {
      return to_string(value.num);
    } else {
      return to_string(value.num) + "/" + to_string(value.den);
    }
  }
};

} // namespace detail
} // namespace mu

#endif
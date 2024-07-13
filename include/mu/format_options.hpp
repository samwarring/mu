#ifndef INCLUDED_MU_FORMAT_OPTIONS_HPP
#define INCLUDED_MU_FORMAT_OPTIONS_HPP

namespace mu {

/// Options control how units should be displayed when formatted as a string.
struct format_options {

  /// Allowed options for displaying named units.
  enum class label_type {

    /// Display named units using their full names.
    names,

    /// Display named units using their symbols.
    ///
    /// If the unit does not have a symbol, its full name is used instead.
    symbols
  };

  /// Controls how to display named units.
  label_type labels = label_type::names;

  /// Separator inserted between units that are multiplied together.
  ///
  /// If two units are multiplied together and the first unit is a prefix,
  /// then the separator is not inserted. However, if the second unit is
  /// unnamed (e.g. a rational value), then the separator is always inserted.
  ///
  /// This is a raw pointer. Users must ensure the string data outlive the
  /// `format_options` that refer to it. For this reason, it is recommended
  /// that `mult_sep` should only be set to string literals, as this avoids
  /// subtle memory-lifetime bugs.
  ///
  const char *mult_sep = " * ";

  /// Whether exponents are formatted as superscript. Default is `false`.
  ///
  /// If `true`, digits of the exponent are formatted as superscript characters.
  /// The characters are encoded as UTF-8. (For example, `meters²`). Note that
  /// some UTF-8 characters do not display well in all contexts. Also,
  /// superscript fractions appear slightly unnatrual (Example: `m¹ᐟ²`). Use at
  /// your own discresion.
  ///
  /// If `false`, exponents are formatted as ASCII digits, joined by the `^`
  /// character. (For example, `meters^2`).
  ///
  bool superscript_exponents = false;
};

} // namespace mu

#endif
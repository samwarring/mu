#ifndef INCLUDED_MU_OSTREAM_HPP
#define INCLUDED_MU_OSTREAM_HPP
#include <mu/detail/unit_string.hpp>
#include <mu/quantity.hpp>
#include <mu/units.hpp>
#include <ostream>

namespace mu {

namespace detail {

/// Holds indices from the "internal extensible array" reserved for holding
/// format options.
///
/// Functions should only obtain instances of this struct from the
/// `get_xalloc_indices` method. They should never construct them directly.
///
struct xalloc_indices {
  int label_type;
  int mult_sep;
  int superscript_exponents;
};

/// Returns indices allocated from the "internal extensible array".
///
/// The indices are initialized statically (only once) on the first invocation.
/// Functions that need to refer to the allocated indices should only do so by
/// calling this function.
///
inline xalloc_indices get_xalloc_indices() {
  static int label_type = std::ios::xalloc();
  static int mult_sep = std::ios::xalloc();
  static int superscript_exponents = std::ios::xalloc();
  xalloc_indices indices;
  indices.label_type = label_type;
  indices.mult_sep = mult_sep;
  indices.superscript_exponents = superscript_exponents;
  return indices;
}

} // namespace detail

namespace stream {

/// Reset all format options set in the stream.
///
/// Quantities inserted in the stream will be formatted according to a default-
/// constructed `format_options`.
///
/// This is a stream manipulator with no arguments. Clients use this method by
/// inserting a function pointer into a stream. For example:
///
///   `std::cout << mu::stream::reset << my_quantity;`
///
inline std::ostream &reset(std::ostream &stream) {
  static auto indices = detail::get_xalloc_indices();
  static format_options default_format;
  stream.iword(indices.label_type) = static_cast<long>(default_format.labels);
  stream.pword(indices.mult_sep) = const_cast<char *>(default_format.mult_sep);
  stream.iword(indices.superscript_exponents) =
      static_cast<long>(default_format.superscript_exponents);
  return stream;
}

/// Sets `format_options` for the stream.
///
/// Quantities inserted in the stream will be formatted according to the
/// provided `format_options`.
///
/// This is a stream manipulator with one argument. Clients use this method by
/// inserting an instance into a stream. For example:
///
///   ```
///   mu::format_options fmt = { ... };
///   std::cout << mu::stream::format(fmt) << my_quantity;
///   ```
///
class format {
public:
  explicit format(const format_options opts) : opts_{opts} {}

  friend std::ostream &operator<<(std::ostream &stream, format manip) {
    static auto indices = detail::get_xalloc_indices();
    stream.iword(indices.label_type) = static_cast<long>(manip.opts_.labels);
    stream.pword(indices.mult_sep) = const_cast<char *>(manip.opts_.mult_sep);
    stream.iword(indices.superscript_exponents) =
        static_cast<long>(manip.opts_.superscript_exponents);
    return stream;
  }

private:
  format_options opts_;
};

/// Sets how named unit labels appear in the stream.
///
/// Quantities inserted in the stream will have their named units formatted
/// according to the provided `label_type`.
///
/// This is a stream manipulator with one argument. Clients use this method by
/// inserting an instance into a stream. For example:
///
///   ```
///   auto ltype = mu::format_options::label_type::symbols;
///   std::cout << mu::stream::labels(ltype) << my_quantity;
///   ```
///
class labels {
public:
  explicit labels(format_options::label_type ltype) : ltype_{ltype} {}

  friend std::ostream &operator<<(std::ostream &stream, labels manip) {
    static auto indices = detail::get_xalloc_indices();
    stream.iword(indices.label_type) = static_cast<long>(manip.ltype_);
    return stream;
  }

private:
  format_options::label_type ltype_;
};

/// Format named units according to their full names.
///
/// This is a stream manipulator with no arguments. Clients use this method by
/// inserting a function pointer into a stream. For example:
///
///   `std::cout << mu::stream::names << my_quantity;`
///
inline std::ostream &names(std::ostream &stream) {
  static auto indices = detail::get_xalloc_indices();
  stream.iword(indices.label_type) =
      static_cast<long>(format_options::label_type::names);
  return stream;
}

/// Format named units according to their symbols.
///
/// This is a stream manipulator with no arguments. Clients use this method by
/// inserting a function pointer into a stream. For example:
///
///   `std::cout << mu::stream::symbols << my_quantity;`
///
inline std::ostream &symbols(std::ostream &stream) {
  static auto indices = detail::get_xalloc_indices();
  stream.iword(indices.label_type) =
      static_cast<long>(format_options::label_type::symbols);
  return stream;
}

/// Sets how unit products appear in the stream.
///
/// Quantities inserted in the stream will have their unit product expressions
/// separated by `sep`.
///
/// This is a stream manipulator with one argument. Clients use this method by
/// inserting an instance into a stream. For example:
///
///   `std::cout << mu::stream::mult_sep(" x ") << my_quantity;`
///
class mult_sep {
public:
  explicit mult_sep(const char *sep) : sep_{sep} {}

  friend std::ostream &operator<<(std::ostream &stream, mult_sep manip) {
    static auto indices = detail::get_xalloc_indices();
    stream.pword(indices.mult_sep) = const_cast<char *>(manip.sep_);
    return stream;
  }

private:
  const char *sep_;
};

/// Sets unit exponents to be displayed as UTF-8 superscripts.
///
/// This is a stream manipulator with no arguments. Clients use this method by
/// inserting a function pointer into a stream. For example:
///
///   `std::cout << mu::stream::superscript_exponents << my_quantity;`
///
inline std::ostream &superscript_exponents(std::ostream &stream) {
  static auto indices = detail::get_xalloc_indices();
  stream.iword(indices.superscript_exponents) = static_cast<long>(true);
  return stream;
}

/// Sets unit exponents to be displayed as ASCII digits, joined to the base with
/// the `^` character.
///
/// This is a stream manipulator with no arguments. Clients use this method by
/// inserting a function pointer into a stream. For example:
///
///   `std::cout << mu::stream::ascii_exponents << my_quantity;`
///
inline std::ostream &ascii_exponents(std::ostream &stream) {
  static auto indices = detail::get_xalloc_indices();
  stream.iword(indices.superscript_exponents) = static_cast<long>(false);
  return stream;
}

} // namespace stream

/// Inserts a formatted quantity into the stream.
///
/// The quantity value is formatted using its overload of the `<<` operator for
/// `std::ostream`. This overload must exist, or this function fails to compile.
///
/// The units are formatted according to a `mu::format_options` object held in
/// the stream. These options can be controlled by various stream manipulators
/// defined by the library.
///
/// \tparam Rep Representation of the quantity.
/// \tparam Units Units of the quantity.
/// \param stream Insert into this stream.
/// \param q Insert this quantity.
///
template <rep Rep, units Units>
std::ostream &operator<<(std::ostream &stream, const quantity<Rep, Units> &q) {
  stream << q.value();

  auto indices = detail::get_xalloc_indices();
  format_options opts;
  long iword_label_type = stream.iword(indices.label_type);
  void *pword_mult_sep = stream.pword(indices.mult_sep);
  long iword_superscript_exponents =
      stream.iword(indices.superscript_exponents);
  if (iword_label_type) {
    opts.labels = static_cast<format_options::label_type>(iword_label_type);
  }
  if (pword_mult_sep) {
    opts.mult_sep = reinterpret_cast<const char *>(pword_mult_sep);
  }
  if (iword_superscript_exponents) {
    opts.superscript_exponents = true;
  }
  stream << ' ' << to_string<Units>(opts);

  return stream;
}

} // namespace mu

#endif